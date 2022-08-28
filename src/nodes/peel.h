#pragma once

#include "statelessProcessor.h"

template <size_t __arityIn, size_t channel>
auto peelFn = [](Frame<__arityIn> input)
{
    return Frame<1> f {input[channel]};
};

template <size_t __arity>
class PeelL : public StatelessProcessor<__arity,peelFn<__arity,0>> { };

template <size_t __arity>
class PeelR : public StatelessProcessor<__arity,peelFn<__arity,1>> { };
