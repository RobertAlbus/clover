#pragma once

#include "node.h"

using namespace Clover::Graph;

namespace Clover::IO {

template <size_t __arity>
class RootNode : public Node<__arity,__arity>
{
public:
    /// RootNode exposes _tick publicly as the starting point of the signal graph
    void _tick(int currentTime) {
        Node<__arity,__arity>::_tick(currentTime);
    }
protected:
    Frame<__arity> tick(Frame<__arity> input) 
    {
        return input;
    }
};

}