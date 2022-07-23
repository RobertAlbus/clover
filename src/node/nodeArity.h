#pragma once
#include <iostream>
#include <numeric>
#include <vector>

#include "frame.h"
#include "frameArity.h"
#include "node.h"
#include "arity.h"

using namespace std;

template <int _arity>
class NodeArity : public Node, public Arity
{
public:
    NodeArity() : Node(), Arity(_arity) {}

    /// User-defined sample processing method
    /// with fallback implementation
    virtual FrameArity<_arity> tick(FrameArity<_arity> input)
    {
        return input;
    }

    /// Manually assign the next value of the Node, overriding
    /// the value received from upstream Nodes
    void next(FrameArity<_arity> next) {
        _hasNext = true;
        _next = next;
    }

    /// Implements Node::current()
    Frame current() {
        return _current;
    }

protected:
    /// Programmatic tick function that gathers the various Frames
    /// from the input Nodes and passes them to implementation-defined tick()
    void _tick(int currentClockTime)
    {
        // Short-circuit graph cycles
        if (currentClockTime == lastComputedClockTime) return;

        lastComputedClockTime = currentClockTime;
        if (_hasNext)
        {   
            tickInputs(currentClockTime);
            _last    = _current;
            _current = _next;
            _hasNext = false;
            return;
        };

        tickInputs(currentClockTime);
        FrameArity<_arity> inputFrame = sumInputs();
        _last    = _current;
        _current = tick(inputFrame);
    }

    /// Advance time for all input nodes
    ///
    void tickInputs(int currentClockTime) {
        for(auto& inputNode : inputNodes) { 
            inputNode->_tick(currentClockTime);
        }
    }

    /// Get a Frame that is arity-matched to this Node
    ///
    FrameArity<_arity> sumInputs() {
        FrameArity<_arity> inputFrame;
        for(auto& inputNode : inputNodes) {
            Frame rawInputFrame = inputNode->current();
            
            FrameArity<_arity> arityMatchedFrame =
                FrameArity<_arity>::From(rawInputFrame);

            inputFrame += arityMatchedFrame;
        }

        return inputFrame;
    }

    FrameArity<_arity> _current;
    FrameArity<_arity> _last;
    FrameArity<_arity> _next;
    bool _hasNext;
    int lastComputedClockTime;
};
