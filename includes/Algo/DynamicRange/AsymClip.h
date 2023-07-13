#pragma once

#include <array>

#include "Algo/AlgorithmBase.h"
#include "Util/Calc.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::DynamicRange {

template <FloatingPoint T, int __arity>
struct AsymClip : public AlgorithmBase<std::array<T, __arity>> {

  AsymClip(float minimum = T(-1), float maximum = T(1))
      : minimum_(minimum), maximum_(maximum) {}

  std::array<T, __arity> process(std::array<T, __arity> input) {
    for (int i = 0; i < __arity; i++) {
      T sign = Util::Calc::sign(input[i]);
      if (sign < 0) {
        this->processed[i] = std::max(minimum_, input[i]);
      } else {
        this->processed[i] = std::min(maximum_, input[i]);
      }
    }
    return this->processed;
  }

  T min() { return minimum_; }
  void min(T minimum) { minimum_ = minimum; }

  T max() { return maximum_; }
  void max(T maximum) { maximum_ = maximum; }

private:
  T minimum_;
  T maximum_;
};

} // namespace Clover::DynamicRange