#pragma once

#include "node.h"

// if I ever encounter arity higher than 2 I should template this.

class PeelL : public Node<2,1>
{
public:
    PeelL() : Node() { }

private:
    Frame<1> tick(Frame<2> input)
    {
        Frame<1> f {input[0]};
        return f;
    }
};

class PeelR : public Node<2,1>
{
public:
    PeelR() : Node() { }

private:
    Frame<1> tick(Frame<2> input)
    {
        Frame<1> f {input[1]};
        return f;
    }
};
