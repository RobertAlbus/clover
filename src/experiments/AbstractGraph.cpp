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
std::function<void(AbstractNode*, std::vector<AbstractNode*>)>
    TypeMap() {
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

// Helper function to add a single type to the map
template<typename T>
void addTypeToMap(std::map<std::type_index, std::function<void(AbstractNode*, std::vector<AbstractNode*>)>>& typeMap) {
    typeMap[typeid(T)] = TypeMap<T>();
}

// Function template to initialize type map with a list of types
template<typename... Types>
void initializeTypeMap(std::map<std::type_index, std::function<void(AbstractNode*, std::vector<AbstractNode*>)>>& typeMap) {
    // Using fold expression to initialize map for each type
    (addTypeToMap<Types>(typeMap), ...);
}


template<Frame FrameType>
std::function<void()>
    InstanceMap(AbstractNode* node, std::vector<AbstractNode*> inputs) {
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


    /*
        TYPE MAP
     */
    // TYPE MAP: setup
    using MapIOFunc = std::function<void(AbstractNode*, std::vector<AbstractNode*>)>;
    std::map<std::type_index, MapIOFunc> typeMap;
                    // typeMap[typeid(IntFrame)] = TypeMap<IntFrame>();
                    // typeMap[typeid(FloatFrame)] = TypeMap<FloatFrame>();
                    // typeMap[typeid(NullFrame)] = TypeMap<NullFrame>();
    // Initialize the map with all frame types in one call
    initializeTypeMap<IntFrame, FloatFrame, NullFrame>(typeMap);
    
    // TYPE MAP: execute
    auto typeMapStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < testIterationCount; i++) {
        typeMap[nodes.at(0)->getTypeIdInput()](nodes.at(0), std::vector<AbstractNode*>{});
        typeMap[nodes.at(1)->getTypeIdInput()](nodes.at(1), std::vector<AbstractNode*>{});
        typeMap[nodes.at(2)->getTypeIdInput()](nodes.at(2), std::vector<AbstractNode*>{});
        typeMap[nodes.at(3)->getTypeIdInput()](nodes.at(3), std::vector<AbstractNode*>{});
        typeMap[nodes.at(4)->getTypeIdInput()](nodes.at(4), std::vector<AbstractNode*>{nodes.at(0), nodes.at(1), nodes.at(2), nodes.at(3)});
        typeMap[nodes.at(5)->getTypeIdInput()](nodes.at(5), std::vector<AbstractNode*>{nodes.at(0), nodes.at(1), nodes.at(2), nodes.at(3)});
        typeMap[nodes.at(6)->getTypeIdInput()](nodes.at(6), std::vector<AbstractNode*>{nodes.at(4), nodes.at(5)});
        typeMap[nodes.at(7)->getTypeIdInput()](nodes.at(7), std::vector<AbstractNode*>{nodes.at(4), nodes.at(5)});
        typeMap[nodes.at(8)->getTypeIdInput()](nodes.at(8), std::vector<AbstractNode*>{nodes.at(6), nodes.at(7)});
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
    std::map<AbstractNode*, std::function<void()>> instanceMap;
    instanceMap[nodes.at(0)] = InstanceMap<NullFrame>(nodes.at(0), std::vector<AbstractNode*>{});
    instanceMap[nodes.at(1)] = InstanceMap<NullFrame>(nodes.at(1), std::vector<AbstractNode*>{});
    instanceMap[nodes.at(2)] = InstanceMap<NullFrame>(nodes.at(2), std::vector<AbstractNode*>{});
    instanceMap[nodes.at(3)] = InstanceMap<NullFrame>(nodes.at(3), std::vector<AbstractNode*>{});
    instanceMap[nodes.at(4)] = InstanceMap<IntFrame>(nodes.at(4), std::vector<AbstractNode*>{nodes.at(0), nodes.at(1), nodes.at(2), nodes.at(3)});
    instanceMap[nodes.at(5)] = InstanceMap<IntFrame>(nodes.at(5), std::vector<AbstractNode*>{nodes.at(0), nodes.at(1), nodes.at(2), nodes.at(3)});
    instanceMap[nodes.at(6)] = InstanceMap<FloatFrame>(nodes.at(6), std::vector<AbstractNode*>{nodes.at(4), nodes.at(5)});
    instanceMap[nodes.at(7)] = InstanceMap<FloatFrame>(nodes.at(7), std::vector<AbstractNode*>{nodes.at(4), nodes.at(5)});
    instanceMap[nodes.at(8)] = InstanceMap<FloatFrame>(nodes.at(8), std::vector<AbstractNode*>{nodes.at(6), nodes.at(7)});

    // TYPE MAP: execute
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