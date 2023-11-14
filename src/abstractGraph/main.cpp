#include <chrono>
#include <concepts>
#include <cstdio>
#include <functional>
#include <latch>
#include <map>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <vector>

#include "FrameBase.h"
#include "NodeBase.h"
#include "CustomTypes.h"



template<Frame FrameType>
std::function<void(AbstractNode*, std::vector<AbstractNode*>&)>
    CreateTypeMapFunction(NodeInput<FrameType> *receivingNode) {
    return [](AbstractNode* node, std::vector<AbstractNode*> inputs) {
        // bulk-casting here is 16% slower vs. cast per in for loop
        FrameType frame {};
        for (auto input : inputs) {
            NodeOutput<FrameType>* providingNode = dynamic_cast<NodeOutput<FrameType>*>(input);
            frame += providingNode->getResult();
        }
        NodeInput<FrameType> *receivingNode = dynamic_cast<NodeInput<FrameType>*>(node);
        receivingNode->processNext(frame);
    };
}


template<Frame FrameType>
std::function<void()>
    InstanceMap(NodeInput<FrameType>* node, std::vector<NodeOutput<FrameType>*> inputs) {

    return [node, inputs]() {
        FrameType frame;
        for (auto input : inputs) {
            frame += input->getResult();
        }
        node->processNext(frame);
    };
}

template<Frame FrameType>
std::function<void()>
    InstanceMap(NodeInput<FrameType>* node, std::vector<AbstractNode*> inputs) {
    std::vector<NodeOutput<FrameType>*> cast_inputs;
    for (auto input : inputs) {
        cast_inputs.emplace_back(dynamic_cast<NodeOutput<FrameType>*>(input));
    }

    return InstanceMap(node, cast_inputs);
}

std::map<AbstractNode*, std::vector<AbstractNode*>> nodeAdjacencyMap;

using MapIOFunc = std::function<void(AbstractNode*, std::vector<AbstractNode*>&)>;
std::map<std::type_index, MapIOFunc> typeMap;
std::map<AbstractNode*, std::function<void()>> instanceMap;

template <Frame X, Frame Y, Frame Z>
Node<Y,Z> &operator>>(Node<X,Y> &sourceNode, Node<Y,Z> &destinationNode) {
    nodeAdjacencyMap[&destinationNode].emplace_back(&sourceNode);

    // type map approach: auto register processing functions
    if (!typeMap.contains(sourceNode.getTypeIdInput())) {
        typeMap[sourceNode.getTypeIdInput()] = CreateTypeMapFunction(&sourceNode);
    }
    if (!typeMap.contains(destinationNode.getTypeIdInput())) {
        typeMap[destinationNode.getTypeIdInput()] = CreateTypeMapFunction(&destinationNode);
    }

    // instance map approach: auto register processing functions
    // - InstanceMap(...) is casting the AbstractNode pointers and capturing the casted adjacency list each time.
    // - can alternatively move a single iteration of the instance map setup into a graph.prepare() method.
    
    if (!instanceMap.contains(&sourceNode)) {
        std::vector<AbstractNode*> inputs = nodeAdjacencyMap[&sourceNode];
        instanceMap[&sourceNode] = InstanceMap(&sourceNode, nodeAdjacencyMap[&sourceNode]);
    }
    instanceMap[&destinationNode] = InstanceMap(&destinationNode, nodeAdjacencyMap[&destinationNode]);


    return destinationNode;
}

int main() {
    int sampleRate = 48000;
    int oneMinute = sampleRate * 60;
    int testIterationCount = oneMinute;

    SourceNode sourceNode1("SN1");
    SourceNode sourceNode2("SN2");
    SourceNode sourceNode3("SN3");
    SourceNode sourceNode4("SN4");
    UpcastNode upcastNode1("UN1");
    UpcastNode upcastNode2("UN2");
    FloatNode floatNode1("FN1");
    FloatNode floatNode2("FN2");
    FloatNode floatNode3("ROOT");

    std::vector<AbstractNode*> nodes;
    nodes.emplace_back(&sourceNode1);
    nodes.emplace_back(&sourceNode2);
    nodes.emplace_back(&sourceNode3);
    nodes.emplace_back(&sourceNode4);
    nodes.emplace_back(&upcastNode1);
    nodes.emplace_back(&upcastNode2);
    nodes.emplace_back(&floatNode1);
    nodes.emplace_back(&floatNode2);
    nodes.emplace_back(&floatNode3);

    for (auto node : nodes) {
        node->reset();
    }

    // a modified topological sort
    // - nodes are grouped into "distance from outside edge"
    // - "outside edge" refers to the farthest upstream node with 0 inputs
    std::map<int, std::vector<AbstractNode*>> sortedNodes;
    sortedNodes[0] = std::vector<AbstractNode*> {
        &sourceNode1,
        &sourceNode2,
        &sourceNode3,
        &sourceNode4,
    };

    sortedNodes[1] = std::vector<AbstractNode*> {
        &upcastNode1,
        &upcastNode2,
    };

    sortedNodes[2] = std::vector<AbstractNode*> {
        &floatNode1,
        &floatNode2,
    };

    sortedNodes[3] = std::vector<AbstractNode*> {
        &floatNode3,
    };

    for (auto node : nodes) {
        std::vector<AbstractNode*> relationships {};
        relationships.reserve(nodes.size());
        nodeAdjacencyMap[node] = relationships;
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
        for (auto group : sortedNodes) {
            for (auto node : group.second) {
                typeMap[node->getTypeIdInput()](node, nodeAdjacencyMap[node]);
            }
        }
    }

    auto typeMapEnd = std::chrono::high_resolution_clock::now();
    auto typeMapDuration = std::chrono::duration_cast<std::chrono::milliseconds>(typeMapEnd - typeMapStart).count();
    printf("\nTime taken |     type map: %6i milliseconds", static_cast<int>(typeMapDuration));

    printf("\nresult: %f", dynamic_cast<NodeOutput<FloatFrame>*>(nodes.back())->getResult().data);
    for (auto node : nodes) {
        node->reset();
    }

    /*
        INSTANCE MAP    
     */
    // INSTANCE MAP: execute
    auto instanceMapStart = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < testIterationCount; i++) {
        for (auto group : sortedNodes) {
            for (auto node : group.second) {
                instanceMap[node]();
            }
        }
    }
    auto instanceMapEnd = std::chrono::high_resolution_clock::now();
    auto instanceMapDuration = std::chrono::duration_cast<std::chrono::milliseconds>(instanceMapEnd - instanceMapStart).count();
    printf("\nTime taken | instance map: %6i milliseconds", static_cast<int>(instanceMapDuration));

    printf("\nresult: %f", dynamic_cast<NodeOutput<FloatFrame>*>(nodes.back())->getResult().data);
    for (auto node : nodes) {
        node->reset();
    }

    printf("\n");
    return 0;
}