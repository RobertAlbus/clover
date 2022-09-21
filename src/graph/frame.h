#pragma once

#include <string>
#include "constants.h"

namespace Clover::Graph {

template<size_t __arity>
using Frame = std::array<Sample, __arity>;

/// add right frame to left frame.
///
template<size_t __arity>
Frame<__arity>& operator+= (Frame<__arity>& frame1, const Frame<__arity>& frame2)
{
    for (int i = 0; i < __arity; i++) {
        frame1[i] += frame2[i];
    }

    return frame1;
}

template<size_t __arity>
Frame<__arity> operator*= (Frame<__arity> frame, float gain)
{
    for (int i = 0; i < __arity; i++) {
        frame[i] *= gain;
    }

    return frame;
}

}