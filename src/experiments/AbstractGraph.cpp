#include <chrono>
#include <concepts>
#include <cstdio>
#include <functional>
#include <map>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <vector>

struct FrameBase {
    FrameBase() = default;
};

template <typename T>
concept Frame = 
    std::is_base_of_v<FrameBase, T> 
    && requires(T a, const T b, const T& c) {
        T();
        { a.clone() } -> std::same_as<T>;
        { b + c } -> std::same_as<T>;
        { a += b } -> std::same_as<T&>;
        
        // { b * c } -> std::same_as<T>;
        // { a *= b } -> std::same_as<void>;
};

struct AbstractNode {
    std::string name;
    AbstractNode(const char *name) : name(std::string(name)) { }
    virtual std::type_index getTypeIdInput() = 0;
    virtual std::type_index getTypeIdOutput() = 0;
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
    
protected: 
    OutputT lastOutput;
};

////////
// example implementation

struct NullFrame : public FrameBase {
    NullFrame() {}
    NullFrame& operator+=(const NullFrame &other) {
        return *(this);
    }
    NullFrame operator+(const NullFrame &other) const {
        return NullFrame();
    }
    NullFrame clone() {
        return NullFrame();
    }
};

struct IntFrame : public FrameBase {
    IntFrame() : data(0) {}
    int data;

    IntFrame& operator+=(const IntFrame &other) {
        data += other.data;
        return *(this);
    }
    IntFrame operator+(const IntFrame &other) const {
        IntFrame f;
        f.data = data + other.data;
        return f;
    }
    IntFrame clone() {
        IntFrame f;
        f.data = data;
        return f;
    }
};

struct FloatFrame : public FrameBase {
    FloatFrame() : data(0.f) {}
    float data;

    FloatFrame& operator+=(const FloatFrame &other) {
        data += other.data;
        return *(this);
    }
    FloatFrame operator+(const FloatFrame &other) const {
        FloatFrame f;
        f.data = data + other.data;
        return f;
    }
    FloatFrame clone() {
        FloatFrame f;
        f.data = data;;
        return f;
    }
};

struct SourceNode : public Node<NullFrame, IntFrame> {
    using Node<NullFrame, IntFrame>::Node;
    IntFrame tick(NullFrame input) {
        IntFrame f;
        f.data = 1;
        return f;
    }
};

struct UpcastNode : public Node<IntFrame, FloatFrame> {
    using Node<IntFrame, FloatFrame>::Node;
    FloatFrame tick(IntFrame input) {
        FloatFrame f;
        f.data = static_cast<float>(input.data);
        return f;
    }
};

struct FloatNode : public Node<FloatFrame, FloatFrame> {
    using Node<FloatFrame, FloatFrame>::Node;
    FloatFrame tick(FloatFrame input) {
        FloatFrame f;
        f.data = input.data + lastOutput.data;
        return f;
    }
};


template<Frame FrameType>
std::function<void(AbstractNode*, std::vector<AbstractNode*>)> MapIO() {
    return [](AbstractNode* node, std::vector<AbstractNode*> inputs) {
        NodeInput<FrameType>* receivingNode = dynamic_cast<NodeInput<FrameType>*>(node);

        FrameType frame {};
        for (auto input : inputs) {

            NodeOutput<FrameType>* providingNode = dynamic_cast<NodeOutput<FrameType>*>(input);
            frame += providingNode->getResult();
        }

        receivingNode->processNext(frame);
    };
}

template<Frame FrameType>
std::function<void()>
    MapIO_v2(AbstractNode* node, std::vector<AbstractNode*> inputs) {
    return [node, inputs]() {
        NodeInput<FrameType>* receivingNode = dynamic_cast<NodeInput<FrameType>*>(node);

        FrameType frame;
        for (auto input : inputs) {
            NodeOutput<FrameType>* providingNode = dynamic_cast<NodeOutput<FrameType>*>(input);
            frame += providingNode->getResult();
        }

        receivingNode->processNext(frame);
    };
}


int main() {
    int testIterationCount = 1000000;

    std::vector<AbstractNode*> nodes;

    SourceNode sourceNode1("SN1");
    SourceNode sourceNode2("SN2");
    UpcastNode upcastNode1("UN1");

    nodes.emplace_back(&sourceNode1);
    nodes.emplace_back(&sourceNode2);
    nodes.emplace_back(&upcastNode1);

    /*
        TYPE MAP
     */

    // TYPE MAP: setup
    using MapIOFunc = std::function<void(AbstractNode*, std::vector<AbstractNode*>)>;
    std::map<std::type_index, MapIOFunc> typeMap;
    typeMap[typeid(IntFrame)] = MapIO<IntFrame>();
    typeMap[typeid(FloatFrame)] = MapIO<FloatFrame>();
    typeMap[typeid(NullFrame)] = MapIO<NullFrame>();
    
    // TYPE MAP: execute
    auto typeMapStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < testIterationCount; i++) {
        typeMap[nodes.at(0)->getTypeIdInput()](nodes.at(0), std::vector<AbstractNode*>{});
        typeMap[nodes.at(1)->getTypeIdInput()](nodes.at(1), std::vector<AbstractNode*>{});
        typeMap[nodes.at(2)->getTypeIdInput()](nodes.at(2), std::vector<AbstractNode*>{nodes.at(0), nodes.at(1)});
    }
    auto typeMapEnd = std::chrono::high_resolution_clock::now();
    auto typeMapDuration = std::chrono::duration_cast<std::chrono::milliseconds>(typeMapEnd - typeMapStart).count();
    printf("\nTime taken |     type map: %6i milliseconds", static_cast<int>(typeMapDuration));

    /*
        INSTANCE MAP    
     */
    // INSTANCE MAP: setup
    std::map<AbstractNode*, std::function<void()>> instanceMap;
    instanceMap[nodes[0]] = MapIO_v2<NullFrame>(nodes[0], std::vector<AbstractNode*>{});
    instanceMap[nodes[1]] = MapIO_v2<NullFrame>(nodes[1], std::vector<AbstractNode*>{});
    instanceMap[nodes[2]] = MapIO_v2<IntFrame>(nodes[2], std::vector<AbstractNode*>{nodes[0], nodes[1]});

    // TYPE MAP: execute
    auto instanceMapStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < testIterationCount; i++) {
        instanceMap[nodes[0]]();
        instanceMap[nodes[1]]();
        instanceMap[nodes[2]]();
    }
    auto instanceMapEnd = std::chrono::high_resolution_clock::now();
    auto instanceMapDuration = std::chrono::duration_cast<std::chrono::milliseconds>(instanceMapEnd - instanceMapStart).count();
    printf("\nTime taken | instance map: %6i milliseconds", static_cast<int>(instanceMapDuration));

    auto outputData = dynamic_cast<NodeOutput<FloatFrame>*>(nodes.at(2))->getResult();
    // printf("\nresult: %f", outputData.data);

    return 0;
}