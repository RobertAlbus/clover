#pragma once
#include <vector>
#include "frame.h"

/// Base class for all N channel nodes of the audio graph.
/// ATTENTION: If you inherit from this class you must also inherit from Arity.
class Node
{
public:

    Node() {}
    /// Add left Node to the right Node.inputNodes
    ///
    Node& operator>> (Node &destinationNode)
    {
        addInputNode(this);
        return destinationNode;
    }

    /// Add left Node to the right Node.inputNodes
    ///
    Node& operator> (Node &destinationNode)
    {
        return (*this) >> destinationNode;
    }

    /// Get current Frame from this Node
    ///
    virtual Frame current()=0; // pure virtual

    virtual void _tick(int currentClockTime)=0;

protected:
    void addInputNode(Node *inputNode)
    {
        inputNodes.emplace_back(inputNode);
    }

    int lastComputedClockTime;
    std::vector<Node*> inputNodes;
};
