#pragma once

#include <algorithm>

#include "node.h"

class Gain : public Node<2,2>
{
public:
    Gain() : Node<2,2>()
    {
    }

private:
    Frame<2> tick(Frame<2> input)
    {
        return input;
    }
};
