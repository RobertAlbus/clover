#pragma once

#include "NodeSimplex.h"

namespace Clover::NodeSimplex::Basic {

template <size_t __arity>
auto invertFn = [](Frame<__arity> input)
{
    Frame<__arity> f;
    // I could try to not be so clever and hand-roll a custom function for each arity. avoid the for-loop overhead.
    for (int i = 0; i < __arity; i++)
    {
        f[i] = input[i] * -1.;
    }

    return f;
};
template <size_t __arity>
class Invert : public StatelessProcessor<__arity,__arity,invertFn<__arity>> { };

}