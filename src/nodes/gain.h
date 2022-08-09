#pragma once

#include <algorithm>

#include "node.h"

template <size_t __arity>
class Gain : public Node<__arity,__arity>
{
public:
    Gain() : Node<__arity,__arity>()
    {
    }

private:
    Frame<__arity> tick(Frame<__arity> input)
    {
        return input;
    }
};

class Gain1 : public Gain<1> {};
class Gain2 : public Gain<2> {};