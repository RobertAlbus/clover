#pragma once

#include <stdexcept>

#include "arity.h"
#include "container.h"
#include "frame.h"
#include "frameHistory.h"


template<int __arityInput>
class ArityInput
{
public:
    ArityInput()
    {

    }

    int arity()
    {
        return _arityIn.arity;
    }
    Arity<__arityInput> _arityIn;

};

template<int __arityOutput>
class ArityOutput
{
public:
    ArityOutput()
    {
        
    }
    int arity()
    {
        return _arityOut.arity;
    }
    Arity<__arityOutput> _arityOut;

};



/// Base class for all N channel nodes of the audio graph.
template<int __arityInput, int __arityOutput>
class Node : public ArityInput<__arityInput>, public ArityOutput<__arityOutput>
{
public:
    float gain;
    Node() :
        ArityInput<__arityInput>(),
        ArityOutput<__arityOutput>(),
        lastComputedClockTime(-1),
        frames(),
        gain(1.)
    {

    }



    /// User-defined sample processing method with fallback implementation
    //
    virtual Frame<__arityOutput> tick(Frame<__arityInput> input) = 0;

    Frame<__arityOutput> current()
    {
        return frames.current();
    }

    void addInputNode(ArityOutput<__arityInput>* inputNode)
    {
        inputNodes << inputNode;
    }

    /// Programmatic tick function that gathers the various Frames
    /// from the input Nodes and passes them to implementation-defined tick()
    void _tick(int currentClockTime)
    {
        // Short-circuit any graph cycles
        if (currentClockTime == lastComputedClockTime) return;

        lastComputedClockTime = currentClockTime;

        tickInputs(currentClockTime);
        frames.current(
            tick( sumInputs() ) 
        );
    }
protected:


    /// Advance time for all input nodes
    ///
    void tickInputs(int currentClockTime) {
    for(int i = 0, end = inputNodes.size(); i < end; i++) {
        // we don't care about the arity of the input node for this operation
        auto inputNode = (Node<0,__arityOutput>*) inputNodes[i]; 
        inputNode->_tick(currentClockTime);
    }
}
    

    /// Get a Frame that is arity-matched to this Node
    ///
    Frame<__arityInput> sumInputs()
    {
        Frame<__arityInput> accumulationFrame;
        for(int i = 0, end = inputNodes.size(); i < end; i++) {
            // cast to arity <0,__arityInput> because we we only care about the output arity of the input nodes.
            auto inputNode = (Node<0,__arityInput>*) inputNodes.getAt(i); 
            accumulationFrame += inputNode->current();
        }
        accumulationFrame *= gain;
        return accumulationFrame;
    }

    Container<ArityOutput<__arityInput>*, 500> inputNodes;
    FrameHistory<__arityOutput> frames;
    int lastComputedClockTime;
    Arity<__arityInput> _arityIn;
    Arity<__arityOutput> _arityOut;

};

/// Add left Node to the right Node.inputNodes
///
template <int X, int Y, int Z>
Node<Y,Z>& operator>> (Node<X,Y> &sourceNode, Node<Y,Z> &destinationNode)
{
    destinationNode.addInputNode(&sourceNode);
    return destinationNode;
}