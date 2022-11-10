#pragma once

#include "graph.h"

using namespace Clover::Graph;

namespace Clover::IO {

template <size_t __arity>
class RootNode : public Node<__arity,__arity>
{
public:
    /// RootNode exposes metaTick publicly as the starting point of the signal graph
    void metaTick(int currentTime) {
        Node<__arity,__arity>::metaTick(currentTime);
    }
protected:
    Frame<__arity> tick(Frame<__arity> input) 
    {
        return input;
    }
};

}