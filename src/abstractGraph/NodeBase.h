#pragma once

#include <string>
#include <typeindex>

#include "FrameBase.h"


struct AbstractNode {
    std::string name;
    AbstractNode(const char *name) : name(std::string(name)) { }
    virtual std::type_index getTypeIdInput() = 0;
    virtual std::type_index getTypeIdOutput() = 0;
    virtual void reset() = 0;
};

template <Frame InputT>
struct NodeInput {
    virtual void processNext(const InputT& input) = 0;
};

template <Frame OutputT>
struct NodeOutput {
    virtual OutputT getResult() = 0;
};

template <Frame InputT, Frame OutputT>
struct Node : public NodeInput<InputT>, public NodeOutput<OutputT>, public AbstractNode {
    Node(const char *nodeName) : AbstractNode(nodeName), lastOutput({}) { }

    void processNext(const InputT& input) override {
        lastOutput = tick(input);
    }

    virtual OutputT tick(InputT input) = 0;

    OutputT getResult() override {
        return lastOutput.clone();
    }

    std::type_index getTypeIdInput() override {
        return typeid(InputT);
    }

    std::type_index getTypeIdOutput() override {
        return typeid(OutputT);
    }

    void reset() override {
        lastOutput.reset();
    }
    
    NodeInput<InputT>* asInput() {
        return dynamic_cast<NodeInput<InputT>*>(this);
    }

    NodeOutput<OutputT>* asOutput() {
        return dynamic_cast<NodeOutput<OutputT>*>(this);
    }
    
protected: 
    OutputT lastOutput;
};