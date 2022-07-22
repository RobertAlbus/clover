#pragma once
#include <iostream>
#include <numeric>
#include <vector>

#include "frame.h"

using namespace std;

template <int _arity>
class Node
{
public:
    Node() : arity(_arity) {}

    /// User-defined sample processing method
    ///
    virtual Frame<_arity> tick(Frame<_arity> input)
    {
        return input
    }

    /// Manually assign the next value of the Node, overriding
    /// the value received from upstream Nodes
    void next(Frame<_arity> next) {
        _next = next;
    }

    /// Get current Frame from this Node
    ///
    Frame<_arity> current() {
        return _current;
    }
    
    /// Add left Node to the right Node.inputNodes
    ///
    Node& operator>> (Node &destinationNode)
    {
        destinationNode.addInputNode(this);
        return destinationNode;
    }

    /// Add left Node to the right Node.inputNodes
    ///
    Node& operator> (Node &destinationNode)
    {
        return (*this) >> destinationNode;
    }

protected:
    void addInputNode(Node *inputNode)
    {
        inputNodes.emplace_back(inputNode);
    }
    
    /// Programmatic tick function that gathers the various Frames
    /// from the input Nodes and passes them to implementation-defined tick()
    Frame<_arity> _tick(int currentClockTime)
    {
        // Short-circuit graph cycles
        if (currentClockTime == lastComputedClockTime) {
            // If we hit this, _current/_last haven't been updated yet.
            // _current is effectively _last and _last is the second-last Frame
            return _current; 
        }

        lastComputedClockTime = currentClockTime;
        if (_next&) != NULL
        {   
            tickInputs(currentClockTime);
            _last    = _current;
            _current = _next;
            _next    = NULL;
            return _current;
        };

        tickInputs(currentClockTime);
        Frame<_arity> inputFrame = sumInputs();
        _last    = _current;
        _current = tick(inputFrame);

        return _current;
    }

    /// Advance time for all input nodes
    ///
    void tickInputs(int currentClockTime) {
        for(auto& inputNode : inputNodes) { 
            (*inputNode)._tick(currentClockTime);
        }
    }

    /// Get a Frame that is arity-matched to this Node
    ///
    Frame<_arity> sumInputs() {
        Frame<_arity> inputFrame;
        for(auto& inputNode : inputNodes) {
            auto arityMatchedFrame = 
                Frame<_arity>::From(inputNode.current());

            inputFrame += arityMatchedFrame
        }

        return inputFrame;
    }

    Frame<_arity> _current;
    Frame<_arity> _last;
    Frame<_arity> _next = NULL;
    int arity;
    int lastComputedClockTime;
    vector<Node*> inputNodes;
};
