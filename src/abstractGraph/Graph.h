#pragma once
#include <vector>
#include <functional>
#include <map>
#include <semaphore>
#include <thread>

#include "FrameBase.h"
#include "NodeBase.h"

struct Graph {
    Graph() {
        if (Graph::context == nullptr) {
            Graph::context = this;
        } 
    }

    // ----------------
    // Context
    static Graph* context;
    void setContext() {
        Graph::context = this;
    }

    // ----------------
    // Type lambdas
    template<Frame FrameType>
    static std::function<void(AbstractNode*, std::vector<AbstractNode*>&)>
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

    // ----------------
    // Instance lambdas
    template<Frame FrameType, Frame Y>
    static std::function<void()>
    InstanceMap(
            Node<FrameType, Y>* node,
            std::vector<NodeOutput<FrameType>*> inputs
            ) {
        Graph* context = Graph::context;
        AbstractNode* node_cast = static_cast<AbstractNode*>(node);

        context->workerWakeupSemaphores[node_cast] = new std::counting_semaphore<>(0);;
        context->returnToMainSemaphores[node_cast] = new std::counting_semaphore<>(0);;

        auto lambda = [context, node, node_cast, inputs]() {
            // printf("\nstarting thread");
            while (true) {
                // printf("\nwaiting");
                context->workerWakeupSemaphores[node_cast]->acquire();

                if (context->isDone) {
                    // printf("\ndone");
                    break;
                }

                FrameType frame;
                for (auto input : inputs) {
                    frame += input->getResult();
                }
                node->processNext(frame);
                // printf("\nsignaling to main");
                context->returnToMainSemaphores[node_cast]->release();
            }
        };

        return lambda;
    }

    template<Frame FrameType, Frame Y>
    static std::function<void()>
        InstanceMap(
            Node<FrameType,Y>* node,
            std::vector<AbstractNode*> inputs
            ) {

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
    std::vector<AbstractNode*> nodes;
    std::map<int, std::vector<AbstractNode*>> sortedNodes;

    std::map<AbstractNode*, std::counting_semaphore<>*> workerWakeupSemaphores;
    std::map<AbstractNode*, std::counting_semaphore<>*> returnToMainSemaphores;
    std::map<AbstractNode*, std::thread> workers;

    bool isDone = false;

};

template <Frame X, Frame Y, Frame Z>
Node<Y,Z> &operator>>(Node<X,Y> &sourceNode, Node<Y,Z> &destinationNode) {
    Graph& graph = *(Graph::context);
    graph.nodeAdjacencyMap[&destinationNode].emplace_back(&sourceNode);

    // type map approach: auto register processing functions
    if (!graph.typeMap.contains(sourceNode.getTypeIdInput())) {
        graph.typeMap[sourceNode.getTypeIdInput()] = Graph::CreateTypeMapFunction(&sourceNode);
    }
    if (!graph.typeMap.contains(destinationNode.getTypeIdInput())) {
        graph.typeMap[destinationNode.getTypeIdInput()] = Graph::CreateTypeMapFunction(&destinationNode);
    }

    // instance map approach: auto register processing functions
    // - InstanceMap(...) is casting the AbstractNode pointers and capturing the casted adjacency list each time.
    // - can alternatively move a single iteration of the instance map setup into a graph.prepare() method.
    
    if (!graph.instanceMap.contains(&sourceNode)) {
        std::vector<AbstractNode*> inputs = graph.nodeAdjacencyMap[&sourceNode];
        graph.instanceMap[&sourceNode] = Graph::InstanceMap(&sourceNode, graph.nodeAdjacencyMap[&sourceNode]);
        // graph.workers[&sourceNode] = {std::move(graph.instanceMap[&sourceNode])};
        // don't do this every invocation, do it once the setup of the graph iw complete
    }
    graph.instanceMap[&destinationNode] = Graph::InstanceMap(&destinationNode, graph.nodeAdjacencyMap[&destinationNode]);

    return destinationNode;
}
