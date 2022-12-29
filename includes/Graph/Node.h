#pragma once

#include <stdexcept>
#include <functional>
#include <vector>

#include "Frame.h"
#include "FrameHistory.h"

namespace Clover::Graph {

template <size_t __arityOutput>
class INode
{
public:
    virtual void metaTick(int currentClockTime) = 0;
    virtual const Frame<__arityOutput>& currentFrame() = 0;
};

/// Base class for all N channel nodes of the audio graph.
template<size_t __arityInput, size_t __arityOutput>
class Node : public INode<__arityOutput>
{
public:
    float _gain;
    float _gainIn;
    const size_t arityInput;
    const size_t arityOutput;
    FrameHistory<__arityOutput> frames;

    Node() :
        arityInput(__arityInput),
        arityOutput(__arityOutput),
        _currentClockTime(-1),
        _gain(1.),
        _gainIn(1.)
    {
        inputNodes.reserve(NODE_MAX_INPUT_CAPACITY);
    }

    void gain(float gainOut)
    {
        _gain = gainOut;
    }
    float gain() {return _gain;}

    void gainIn(float gainIn)
    {
        _gainIn = gainIn;
    }
    float gainIn() { return _gainIn;}

    std::vector<INode<__arityInput>*> inputNodes;
    int _currentClockTime;

    /// User-defined sample processing method with fallback implementation
    //
    virtual Frame<__arityOutput> tick(Frame<__arityInput> input) = 0;

    template<size_t X>
    void addInputNode(Node<X,__arityInput> &inputNode)
    {
        if(std::find(inputNodes.begin(), inputNodes.end(), &inputNode) != inputNodes.end())
        {
            printf("\nAttempted to double connect %p to %p\n", &inputNode, this);
            exit(1);
        } 
        inputNodes.emplace_back(&inputNode);
    }

    /// Programmatic tick function that gathers the various Frames
    /// from the input Nodes and passes them to implementation-defined tick()
    void metaTick(int currentClockTime)
    {
        // Short-circuit any graph cycles
        if (currentClockTime == _currentClockTime) return;

        _currentClockTime = currentClockTime;
        tickInputs(currentClockTime);
        tickCallback(currentClockTime);

        for(int i = 0, end = __arityInput; i < end; i++) {
            accumulationFrame[i] = 0;
        }
        for(int i = 0, end = inputNodes.size(); i < end; i++) {
            accumulationFrame += (inputNodes.at(i))->currentFrame();
        }
        accumulationFrame *= _gainIn; 

        processedFrame = tick( accumulationFrame );

        processedFrame *= _gain;
        frames.push( processedFrame );
    }

    const Frame<__arityOutput>& currentFrame()
    {
        return frames.current;
    }

private:
    Frame<__arityInput> accumulationFrame = {};
    Frame<__arityOutput> processedFrame {};
    /// Advance time for all input nodes
    ///
    void tickInputs(int currentClockTime) {
        for(int i = 0, end = inputNodes.size(); i < end; i++) {
            (inputNodes.at(i))->metaTick(currentClockTime);
        }
    }

    /// override this method to add functionality to hook into Node::metaTick(int currentClockTime)
    virtual void tickCallback(int currentClockTime) { }

};

/// Add left Node to the right Node.inputNodes
///
template<size_t X, size_t Y, size_t Z>
Node<Y,Z>& operator>> (Node<X,Y> &sourceNode, Node<Y,Z> &destinationNode)
{
    printf("\n%p   >> %p", &sourceNode, &destinationNode);
    destinationNode.addInputNode(sourceNode);
    return destinationNode;
}

template<size_t X, size_t Y, size_t Z>
Node<Y,Z>& operator>> (Node<X,Y> *sourceNode, Node<Y,Z> &destinationNode)
{
    return *(sourceNode) >> destinationNode;
}

template<size_t X, size_t Y, size_t Z>
Node<Y,Z>& operator>> (Node<X,Y> &sourceNode, Node<Y,Z> *destinationNode)
{
    return sourceNode >> *(destinationNode);
}

}