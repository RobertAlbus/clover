#pragma once

#include "graph/frame.h"
#include "NodeSimplex.h"

using namespace Clover::Graph;

namespace Clover::NodeSimplex::Adapter {

template <size_t __arityIn, size_t __arityOut>
auto adapterFn = [](Frame<__arityIn> input)
{
    return Frame<__arityOut> {};
};
template <size_t __arityIn, size_t __arityOut>
class NullAdapter : public StatelessProcessor<__arityIn,__arityOut, adapterFn<__arityIn,__arityOut>> { };

}