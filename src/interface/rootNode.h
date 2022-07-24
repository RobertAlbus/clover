#pragma once
#include "node.h"

class RootNode : public Node
{
public:
    RootNode();
    Frame tick(Frame input);

    /// RootNode exposes _tick publicly as the starting point of the signal graph
    void _tick(int currentTime);
};