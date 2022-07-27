#pragma once

#include <stdexcept>
#include <vector>

#include "arity.h"
#include "container.h"
#include "frame.h"

/// Base class for all N channel nodes of the audio graph.
class Node
{
public:
    Node(int arityIn, int arityOut);

    /// Add left Node to the right Node.inputNodes
    ///
    Node& operator>> (Node &destinationNode);

    /// User-defined sample processing method with fallback implementation
    //
    virtual Frame tick(Frame input) = 0;

    /// Manually assign the next value of the Node, overriding the value received from upstream Nodes
    //
    void next(Frame next);

    Frame current();
    int arityIn();
    int arityOut();

protected:
    void addInputNode(Node* inputNode);

    /// Programmatic tick function that gathers the various Frames
    /// from the input Nodes and passes them to implementation-defined tick()
    void _tick(int currentClockTime);

    /// Advance time for all input nodes
    ///
    void tickInputs(int currentClockTime);
    

    /// Get a Frame that is arity-matched to this Node
    ///
    Frame sumInputs();

    Frame _current;
    Frame _last;
    Frame _next;
    bool _hasNext;
    int lastComputedClockTime;
    Container<Node*, 500> inputNodes;
    Arity _arityIn;
    Arity _arityOut;
};
