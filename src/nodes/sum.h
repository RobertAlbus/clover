#pragma once

#include "node.h"

class Sum1 : public Node<2,1>
{
public:
    Sum1() : Node<2,1>() { }

private:
    Frame<1> tick(Frame<2> input)
    {
        Frame<1> f {(input[0] + input[1]) * 0.5};
        return f;
    }
};

class Sum2 : public Node<2,2>
{
public:
    Sum2() : Node<2,2>() { }

private:
    Frame<2> tick(Frame<2> input)
    {
        Frame<2> f {
            (input[0] + input[1]) * 0.5,
            (input[0] + input[1]) * 0.5
        };

        return f;
    }
};
