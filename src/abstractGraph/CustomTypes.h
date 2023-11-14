#pragma once

#include "FrameBase.h"
#include "NodeBase.h"


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