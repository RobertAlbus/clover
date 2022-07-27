
#include <stdexcept>

#include "arity.h"
#include "frame.h"
#include "node.h"


Node::Node(int arityIn, int arityOut) : 
    _arityIn(Arity(arityIn)),
    _arityOut(Arity(arityOut)),
    _last(Frame(arityOut)),
    _current(Frame(arityOut)),
    _next(Frame(arityOut)),
    _hasNext(false),
    lastComputedClockTime(-1)
{
    
}

Node& Node::operator>> (Node &destinationNode)
{
    destinationNode.addInputNode(this);
    return destinationNode;
}

void Node::next(Frame next) {
    if (arityOut() != next.arity) {
        throw std::domain_error("cannot call Node::next(Frame) with mismatched arities");
    }
    _hasNext = true;
    _next = next;
}

Frame Node::current() {
    return _current;
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
        Frame inputNodeFrame = inputNode->current();
        Frame arityMatchedFrame = inputNodeFrame.convertArity(arityIn());

        accumulationFrame += arityMatchedFrame;
    }

    return accumulationFrame;
}
