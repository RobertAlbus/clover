#include <chrono>
#include <vector>
#include <functional>
#include <map>

#include "FrameBase.h"
#include "NodeBase.h"
#include "CustomTypes.h"
#include "Graph.h"


int main() {
    Graph graph;

    int sampleRate = 48000;
    int oneMinute = sampleRate * 60;
    int testIterationCount = 100000;

    SourceNode sourceNode1("SN1");
    SourceNode sourceNode2("SN2");
    SourceNode sourceNode3("SN3");
    SourceNode sourceNode4("SN4");
    UpcastNode upcastNode1("UN1");
    UpcastNode upcastNode2("UN2");
    FloatNode floatNode1("FN1");
    FloatNode floatNode2("FN2");
    FloatNode floatNode3("ROOT");

    graph.nodes.emplace_back(&sourceNode1);
    graph.nodes.emplace_back(&sourceNode2);
    graph.nodes.emplace_back(&sourceNode3);
    graph.nodes.emplace_back(&sourceNode4);
    graph.nodes.emplace_back(&upcastNode1);
    graph.nodes.emplace_back(&upcastNode2);
    graph.nodes.emplace_back(&floatNode1);
    graph.nodes.emplace_back(&floatNode2);
    graph.nodes.emplace_back(&floatNode3);

    for (auto node : graph.nodes) {
        node->reset();
    }

    // a modified topological sort
    // - nodes are grouped into "distance from outside edge"
    // - "outside edge" refers to the farthest upstream node with 0 inputs
    // NOTE: This sort will be implemented later
    graph.sortedNodes[0] = {
        &sourceNode1,
        &sourceNode2,
        &sourceNode3,
        &sourceNode4,
    };

    graph.sortedNodes[1] = {
        &upcastNode1,
        &upcastNode2,
    };

    graph.sortedNodes[2] = {
        &floatNode1,
        &floatNode2,
    };

    graph.sortedNodes[3] = {
        &floatNode3,
    };

    for (auto node : graph.nodes) {
        graph.nodeAdjacencyMap[node] = {};
        graph.nodeAdjacencyMap[node].reserve(graph.nodes.size());
    }

    sourceNode1 >> upcastNode1;
    sourceNode2 >> upcastNode1;
    sourceNode3 >> upcastNode1;
    sourceNode4 >> upcastNode1;
    sourceNode1 >> upcastNode2;
    sourceNode2 >> upcastNode2;
    sourceNode3 >> upcastNode2;
    sourceNode4 >> upcastNode2;

    upcastNode1 >> floatNode1;
    upcastNode2 >> floatNode1;
    upcastNode1 >> floatNode2;
    upcastNode2 >> floatNode2;

    floatNode1 >> floatNode3;
    floatNode2 >> floatNode3;


    /*
        TYPE MAP
     */
    // TYPE MAP: execute
    auto typeMapStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < testIterationCount; i++) {
        for (auto group : graph.sortedNodes) {
            for (auto node : group.second) {
                graph.typeMap[node->getTypeIdInput()](node, graph.nodeAdjacencyMap[node]);
            }
        }
    }

    auto typeMapEnd = std::chrono::high_resolution_clock::now();
    auto typeMapDuration = std::chrono::duration_cast<std::chrono::milliseconds>(typeMapEnd - typeMapStart).count();
    printf("\nTime taken |     type map: %6i milliseconds", static_cast<int>(typeMapDuration));

    printf("\nresult: %f", dynamic_cast<NodeOutput<FloatFrame>*>(graph.nodes.back())->getResult().data);
    for (auto node : graph.nodes) {
        node->reset();
    }

    /*
        INSTANCE MAP    
     */
    // INSTANCE MAP: execute

    for (auto node : graph.nodes) {
        graph.workers[node] = std::thread(graph.instanceMap[node]);
    }

    auto instanceMapStart = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < testIterationCount; i++) {
        for (auto group : graph.sortedNodes) {
            for (auto node : group.second) {
                Graph::context->workerWakeupSemaphores[node]->release();
            }
            for (auto node : group.second) {
                Graph::context->returnToMainSemaphores[node]->acquire();
            }
        }
    }

    graph.isDone = true;
    for (auto node : graph.nodes) {
        Graph::context->workerWakeupSemaphores[node]->release();
    }

    for (auto node : graph.nodes) {
        Graph::context->workers[node].join();
    }

    auto instanceMapEnd = std::chrono::high_resolution_clock::now();
    auto instanceMapDuration = std::chrono::duration_cast<std::chrono::milliseconds>(instanceMapEnd - instanceMapStart).count();
    printf("\nTime taken | instance map: %6i milliseconds", static_cast<int>(instanceMapDuration));

    printf("\nresult: %f", dynamic_cast<NodeOutput<FloatFrame>*>(graph.nodes.back())->getResult().data);
    for (auto node : graph.nodes) {
        node->reset();
    }

    printf("\n");
    return 0;
}