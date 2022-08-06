#pragma once

#include <stdexcept>
#include <functional>
#include <vector>

#include "frame.h"
#include "frameHistory.h"

template <size_t __arityOutput>
class INode
{
public:
    virtual void _tick(int currentClockTime) = 0;
    virtual Frame<__arityOutput> getCurrentFrame() = 0;
};

/// Base class for all N channel nodes of the audio graph.
template<size_t __arityInput, size_t __arityOutput>
class Node : public INode<__arityOutput>
{
public:
    float gain;
    const size_t arityInput;
    const size_t arityOutput;
    FrameHistory<__arityOutput> frames;

    Node() :
        arityInput(__arityInput),
        arityOutput(__arityOutput),
        lastComputedClockTime(-1),
        gain(1.)
    {
    }

    /// User-defined sample processing method with fallback implementation
    //
    virtual Frame<__arityOutput> tick(Frame<__arityInput> input) = 0;

    template<size_t X>
    void addInputNode(Node<X,__arityInput> &inputNode)
    {
        inputNodes.emplace_back(&inputNode);
    }

    /// Programmatic tick function that gathers the various Frames
    /// from the input Nodes and passes them to implementation-defined tick()
    void _tick(int currentClockTime)
    {
        // Short-circuit any graph cycles
        if (currentClockTime == lastComputedClockTime) return;

        lastComputedClockTime = currentClockTime;

        tickInputs(currentClockTime);
        frames.push(
            tick( sumInputs() ) *= gain
        );
    }

    Frame<__arityOutput> getCurrentFrame()
    {
        return frames.current;
    }
protected:


    /// Advance time for all input nodes
    ///
    void tickInputs(int currentClockTime) {
    for(int i = 0, end = inputNodes.size(); i < end; i++) {
        (inputNodes.at(i))->_tick(currentClockTime);
    }
}
    

    /// Get a Frame that is arity-matched to this Node
    ///
    Frame<__arityInput> sumInputs()
    {
        Frame<__arityInput> accumulationFrame = {};
        for(int i = 0, end = inputNodes.size(); i < end; i++) {
            accumulationFrame += (inputNodes.at(i))->getCurrentFrame();
        }
        accumulationFrame;
        return accumulationFrame;
    }
public:
    std::vector<INode<__arityInput>*> inputNodes;
    int lastComputedClockTime;

};

/// Add left Node to the right Node.inputNodes
///
template<size_t X, size_t Y, size_t Z>
Node<Y,Z>& operator>> (Node<X,Y> &sourceNode, Node<Y,Z> &destinationNode)
{
    printf("\n%p   >> %p", &sourceNode, &destinationNode);
    auto destinationNodeAddress = &destinationNode;
    auto sourceNodeAddress = &sourceNode;
    destinationNode.addInputNode(sourceNode);
    return destinationNode;
}