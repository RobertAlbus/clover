#pragma once

#include <array>

#include "Algo/AlgorithmBase.h"
#include "Util/Calc.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::DynamicRange {

template <FloatingPoint T, int __arity>
struct Clip : public AlgorithmBase<std::array<T, __arity>> {

  Clip(float threshold = 1.f) : threshold_(threshold) {}

  std::array<T, __arity> process(std::array<T, __arity> input) {
    std::array<T, __arity> frame{};
    for (int i = 0; i < __arity; i++) {
      T sign = Util::Calc::sign(input[i]);
      T absolute = abs(input[i]);

      T result = absolute < threshold_ ? absolute : threshold_;
      result *= sign;

      frame[i] = result;
    }
    this->processed = frame;
    return this->processed;
  }

  void threshold(T t) { threshold_ = t; }

private:
  T threshold_;
};

} // namespace Clover::DynamicRange
