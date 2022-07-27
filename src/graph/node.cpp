
#include <stdexcept>

#include "arity.h"
#include "frame.h"
#include "node.h"


Node::Node(int arityIn, int arityOut) : 
    _arityIn(Arity(arityIn)),
    _arityOut(Arity(arityOut)),
    lastComputedClockTime(-1),
    frames(FrameHistory(arityOut))
{
    
}

Node& Node::operator>> (Node &destinationNode)
{
    if (arityOut() != destinationNode.arityIn()) {
        throw std::domain_error("cannot connect nodes with mismatched input and output arities");
    }
    destinationNode.addInputNode(this);
    return destinationNode;
}

void Node::next(Frame n) {
    if (arityOut() != n.arity) {
        throw std::domain_error("cannot call Node::next(Frame) with mismatched arities");
    }
    frames.next(n);
}

Frame Node::current() {
    return frames.current();
}

int Node::arityIn() {
    return _arityIn.arity;
}
int Node::arityOut() {
    return _arityOut.arity;
}

void Node::addInputNode(Node* inputNode)
{
    inputNodes << inputNode;
}

void Node::_tick(int currentClockTime)
{
    // Short-circuit any graph cycles
    if (currentClockTime == lastComputedClockTime) return;

    lastComputedClockTime = currentClockTime;
    if (frames.hasNext())
    {   
        tickInputs(currentClockTime);
        frames.tick();
        return;
    };

    tickInputs(currentClockTime);
    frames.current(
        tick( sumInputs() )
    );
}

void Node::tickInputs(int currentClockTime) {
    for(int i = 0, end = inputNodes.size(); i < end; i++) {
        Node* inputNode = inputNodes[i]; 
        inputNode->_tick(currentClockTime);
    }
}

Frame Node::sumInputs() {
    Frame accumulationFrame(arityIn());
    for(int i = 0, end = inputNodes.size(); i < end; i++) {
        Node* inputNode = inputNodes.getAt(i); 
        accumulationFrame += inputNode->current();
    }

    return accumulationFrame;
}
