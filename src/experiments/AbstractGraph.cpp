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
        { a.reset() } -> std::same_as<void>;
        
        // { b * c } -> std::same_as<T>;
        // { a *= b } -> std::same_as<void>;
};

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
    void reset() {}
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
    void reset() {
        data = 0;
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
    void reset() {
        data = 0.f;
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
    
    instanceMap[&sourceNode] = InstanceMap(&sourceNode, nodeAdjacencyMap[&sourceNode]);
    instanceMap[&destinationNode] = InstanceMap(&destinationNode, nodeAdjacencyMap[&destinationNode]);

    // an ordering issue here!?!?!?!
    // nope
    // InstanceMap is casting the AbstractNode pointers and capturing the casted adjacency list each time.

    // if (!instanceMap.contains(&sourceNode)) {
    //     std::vector<AbstractNode*> inputs = nodeAdjacencyMap[&sourceNode];
    //     instanceMap[&sourceNode] = InstanceMap(&sourceNode, nodeAdjacencyMap[&sourceNode]);
    // }

    // if (!instanceMap.contains(&destinationNode)) {
    //     std::vector<AbstractNode*> inputs = nodeAdjacencyMap[&destinationNode];
    //     instanceMap[&destinationNode] = InstanceMap(&destinationNode, nodeAdjacencyMap[&destinationNode]);
    // }

    // can alternatively move a single iteration of the instance map setup into a graph.prepare() method.

    return destinationNode;
}

int main() {
    int sampleRate = 48000;
    int oneMinute = sampleRate * 60;
    int testIterationCount = oneMinute;

    std::vector<AbstractNode*> nodes;

    SourceNode sourceNode1("SN1");
    SourceNode sourceNode2("SN2");
    SourceNode sourceNode3("SN3");
    SourceNode sourceNode4("SN4");
    UpcastNode upcastNode1("UN1");
    UpcastNode upcastNode2("UN2");
    FloatNode floatNode1("FN1");
    FloatNode floatNode2("FN2");
    FloatNode floatNode3("ROOT");

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
    // TYPE MAP: setup
    
    // TYPE MAP: execute
    auto typeMapStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < testIterationCount; i++) {
        for (auto node : nodes) {
            typeMap[node->getTypeIdInput()](node, nodeAdjacencyMap[node]);
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
    // INSTANCE MAP: setup

    // INSTANCE MAP: execute
    auto instanceMapStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < testIterationCount; i++) {
        for (auto node : nodes) {
            instanceMap[node]();
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