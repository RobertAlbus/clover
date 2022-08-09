#pragma once
#include "gain.h"

template <size_t __arity>
class RootNode : public Node<__arity,__arity>
{
public:

    Frame<__arity> tick(Frame<__arity> input) 
    {
        return input;
    }

    /// RootNode exposes _tick publicly as the starting point of the signal graph
    void _tick(int currentTime) {
        Node<__arity,__arity>::_tick(currentTime);
    }
};
