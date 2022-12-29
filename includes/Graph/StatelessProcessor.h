#pragma once

#include <functional>

#include "Graph.h"

namespace Clover::Graph
{

/// A Node with a transform function that does not maintain any inter-sample state.
template <size_t __arityIn, size_t __arityOut, Frame<__arityOut> processFn(Frame<__arityIn>)> 
class StatelessProcessor : public Node<__arityIn, __arityOut>
{
public:
  StatelessProcessor() : Node<__arityIn, __arityOut>() {}
protected:
    Frame<__arityOut> tick(Frame<__arityIn> input)
    {
        return processFn(input);
    };
};

}
