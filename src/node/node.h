#pragma once
#include <vector>
#include "frame.h"

/// Base class for all N channel nodes of the audio graph.
class Node : public Arity
{
public:
    Node() : Arity() {

    }
    Node(int _arity) : 
        Arity(_arity),
        _last(Frame(_arity)),
        _current(Frame(_arity)),
        _next(Frame(_arity)),
        _hasNext(false),
        lastComputedClockTime(-1)
    {
        
    }

    /// Add left Node to the right Node.inputNodes
    ///
    Node& operator>> (Node &destinationNode)
    {
        destinationNode.addInputNode(this);
        return destinationNode;
    }

    /// User-defined sample processing method
    /// with fallback implementation
    virtual Frame tick(Frame input) = 0;

    /// Manually assign the next value of the Node, overriding
    /// the value received from upstream Nodes
    void next(Frame next) {
        // TODO: if arity doesn't match, convert and log? or throw? 
        if (getArity() != next.getArity()) {
            throw std::domain_error("cannot call Node::next(Frame) with mismatched arities");
        }
        _hasNext = true;
        _next = next;
    }

    /// Implements Node::current()
    Frame current() {
        return _current;
    }

protected:
    void addInputNode(Node* inputNode)
    {
        inputNodes.emplace_back(inputNode);
    }

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
        Frame inputFrame = sumInputs();
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
    Frame sumInputs() {
        Frame accumulationFrame(getArity());
        for(auto& inputNode : inputNodes) {
            Frame inputNodeFrame = inputNode->current();
            Frame arityMatchedFrame = inputNodeFrame.convertArity(getArity());

            accumulationFrame += arityMatchedFrame;
        }

        return accumulationFrame;
    }

    Frame _current;
    Frame _last;
    Frame _next;
    bool _hasNext;
    int lastComputedClockTime;
    std::vector<Node*> inputNodes;
};
