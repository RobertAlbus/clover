#pragma once
#include <vector>
#include <functional>
#include <map>

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
    template<Frame FrameType>
    static std::function<void()>
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
    static std::function<void()>
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
    std::vector<AbstractNode*> nodes;
    std::map<int, std::vector<AbstractNode*>> sortedNodes;

};
