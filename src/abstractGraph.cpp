#include <concepts>
#include <cstdio>
#include <type_traits>
#include <vector>

struct FrameBase {
    FrameBase() = default;
    // FrameBase(const FrameBase&) = default;
    // virtual FrameBase& operator+=(const FrameBase &other) = 0;
    // virtual FrameBase operator+(const FrameBase &other) = 0;
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
    // virtual void processNext(const FrameBase& input) = 0;
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
    // virtual void processNext(const FrameBase& input) override {
    //     InputT *cast = reinterpret_cast<const InputT*>(&input);
    //     processNext(*(cast));
    // }

    void processNext(const InputT& input) override {
        lastOutput = tick(input);
    }

    virtual OutputT tick(InputT input) = 0;

    OutputT getResult() override {
        return lastOutput.clone();
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
    IntFrame() {}
    int data = 0;

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
        f.data = data;;
        return f;
    }
};

struct FloatFrame : public FrameBase {
    FloatFrame() {}
    float data = 0;

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
    IntFrame tick(NullFrame input) {
        IntFrame f;
        f.data = 1;
        return f;
    }
};

struct UpcastNode : public Node<IntFrame, FloatFrame> {
    FloatFrame tick(IntFrame input) {
        FloatFrame f;
        f.data = static_cast<float>(input.data);
        return f;
    }
};

struct FloatNode : public Node<FloatFrame, FloatFrame> {
    FloatFrame tick(FloatFrame input) {
        FloatFrame f;
        f.data = input.data + lastOutput.data;
        return f;
    }
};



int main() {

    std::vector<AbstractNode*> nodes;

    SourceNode sourceNode1;
    SourceNode sourceNode2;
    UpcastNode upcastNode1;

    nodes.emplace_back(&sourceNode1);
    nodes.emplace_back(&sourceNode2);
    nodes.emplace_back(&upcastNode1);

    nodes.at(0) = &sourceNode1;
    nodes.at(1) = &sourceNode2;
    nodes.at(2) = &upcastNode1;

    ////
    // source node 1
    auto sourceNode1_reinterpret_input = reinterpret_cast<NodeInput<NullFrame>*>(nodes.at(0));
    sourceNode1_reinterpret_input->processNext(NullFrame());

    auto sourceNode1_reinterpret_output = reinterpret_cast<NodeOutput<IntFrame>*>(nodes.at(0));
    auto sourceNode1_output = sourceNode1_reinterpret_output->getResult();

    ////
    // source node 2
    auto sourceNode2_reinterpret_input = reinterpret_cast<NodeInput<NullFrame>*>(nodes.at(1));
    sourceNode2_reinterpret_input->processNext(NullFrame());

    auto sourceNode2_reinterpret_output = reinterpret_cast<NodeOutput<IntFrame>*>(nodes.at(1));
    auto sourceNode2_output = sourceNode2_reinterpret_output->getResult();

    ////
    // upcast node 1
    auto upcastNode1_reinterpret_input = reinterpret_cast<NodeInput<IntFrame>*>(nodes.at(2));
    upcastNode1_reinterpret_input->processNext(sourceNode1_output + sourceNode2_output);

    auto upcastNode1_reinterpret_output = reinterpret_cast<NodeOutput<FloatFrame>*>(nodes.at(1));
    auto upcastNode1_output = upcastNode1_reinterpret_output->getResult();

    printf("\nresult: %f", upcastNode1_output.data);

    return 0;
}