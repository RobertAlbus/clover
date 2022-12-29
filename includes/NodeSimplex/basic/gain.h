#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Basic {

template <size_t __arity>
auto passFn = [](Frame<__arity> input)
{
    return input;
};
template <size_t __arity>
class Gain : public StatelessProcessor<__arity,__arity,passFn<__arity>> { };

}
