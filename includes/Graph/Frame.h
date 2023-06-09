#pragma once

#include <array>

#include "Constants.h"

namespace Clover::Graph {

template <size_t __arity> using Frame = std::array<Sample, __arity>;

template <size_t __arity, size_t size>
using FrameBuffer = std::array<Frame<__arity>, size>;

/// add right frame to left frame.
///
template <size_t __arity>
void operator+=(Frame<__arity> &frame1,
                           const Frame<__arity> &frame2) {
  for (int i = 0; i < __arity; i++) {
    frame1[i] += frame2[i];
  }
}

template <size_t __arity>
void operator*=(Frame<__arity> &frame, float gain) {
  for (int i = 0; i < __arity; i++) {
    frame[i] *= gain;
  }
}

template <size_t __arity>
void operator*=(Frame<__arity> &frame1,
                           const Frame<__arity> &frame2) {
  for (int i = 0; i < __arity; i++) {
    frame1[i] *= frame2[i];
  }
}

} // namespace Clover::Graph