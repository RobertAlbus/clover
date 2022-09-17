#pragma once

#include "frame.h"
#include "statelessProcessor.h"
#include "../nodeSimplex.h"

namespace Clover::NodeSimplex::Basic {

template <size_t __arity>
auto passFn = [](Frame<__arity> input)
{
    return input;
};
template <size_t __arity>
class Gain : public StatelessProcessor<__arity,__arity,passFn<__arity>> { };

class Gain1 : public Gain<1> {};
class Gain2 : public Gain<2> {};

}
