#pragma once

#include <stdexcept>

#include <functional>
#include <vector>
#include "frame.h"
#include "frameHistory.h"


template<size_t __arity>
class ArityInput
{
public:
    ArityInput() : arity(__arity) { }
    const size_t arity;
};

template<size_t __arity>
class ArityOutput
{
public:
    ArityOutput() : arity(__arity) { }
    const size_t arity;
};



/// Base class for all N channel nodes of the audio graph.
template<size_t __arityInput, size_t __arityOutput>
class Node : public ArityInput<__arityInput>, public ArityOutput<__arityOutput>
{
public:
    float gain;
    FrameHistory<__arityOutput> frames;

    Node() :
        ArityInput<__arityInput>(),
        ArityOutput<__arityOutput>(),
        lastComputedClockTime(-1),
        gain(1.)
    {

    }


    /// User-defined sample processing method with fallback implementation
    //
    virtual Frame<__arityOutput> tick(Frame<__arityInput> input) = 0;


    void addInputNode(ArityOutput<__arityInput>* inputNode)
    {
        inputNodes.emplace_back(inputNode);
    }

    /// Programmatic tick function that gathers the various Frames
    /// from the input Nodes and passes them to implementation-defined tick()
    void _tick(int currentClockTime)
    {
        // Short-circuit any graph cycles
        if (currentClockTime == lastComputedClockTime) return;
        // printf("\n%p  <======================== \n", this);

        lastComputedClockTime = currentClockTime;

        tickInputs(currentClockTime);
        frames.push(
            tick( sumInputs() ) *= gain
        );
    }
protected:


    /// Advance time for all input nodes
    ///
    void tickInputs(int currentClockTime) {
    for(int i = 0, end = inputNodes.size(); i < end; i++) {
        // we don't care about the arity of the input node for this operation
        auto inputNode = (Node<0,__arityOutput>*) inputNodes.at(i); 
        inputNode->_tick(currentClockTime);
    }
}
    

    /// Get a Frame that is arity-matched to this Node
    ///
    Frame<__arityInput> sumInputs()
    {
        Frame<__arityInput> accumulationFrame = {};
        for(int i = 0, end = inputNodes.size(); i < end; i++) {
            // cast to arity <0,__arityInput> because we we only care about the output arity of the input nodes.
            auto inputNode = (Node<0,__arityInput>*) inputNodes.at(i); 
            accumulationFrame += (*inputNode).frames.current;
        }
        accumulationFrame;
        return accumulationFrame;
    }
public:
    std::vector<ArityOutput<__arityInput>*> inputNodes;
    int lastComputedClockTime;

};

/// Add left Node to the right Node.inputNodes
///
template<size_t X, size_t Y, size_t Z>
Node<Y,Z>& operator>> (Node<X,Y> &sourceNode, Node<Y,Z> &destinationNode)
{
    printf("\n%p >> %p", &sourceNode, &destinationNode);
    destinationNode.addInputNode(&sourceNode);
    return destinationNode;
}