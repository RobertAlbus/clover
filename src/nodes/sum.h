#pragma once

#include "node.h"

class Sum : public Node<2,1>
{
public:
    Sum() : Node<2,1>() { }

private:
    Frame<1> tick(Frame<2> input)
    {
        Frame<1> f {(input[0] + input[1]) * 0.5};
        return f;
    }
};
