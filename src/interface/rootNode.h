#pragma once
#include "node.h"

class RootNode : public Node<2,2>
{
public:
    RootNode() : Node<2,2>()
    {

    }

    Frame<2> tick(Frame<2> input) 
    {
        return input;
    }

    /// RootNode exposes _tick publicly as the starting point of the signal graph
    void _tick(int currentTime) {
        Node::_tick(currentTime);
    }
};
