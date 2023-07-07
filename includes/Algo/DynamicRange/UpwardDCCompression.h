#pragma once

#include <array>

#include "Algo/AlgorithmBase.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::DynamicRange {

template <FloatingPoint T, int __arity>
struct UpwardDCCompression : public AlgorithmBase<std::array<T, __arity>> {

  UpwardDCCompression(T threshold = 0.f) { this->threshold(threshold); }

  std::array<T, __arity> process(std::array<T, __arity> input) {
    std::array<T, __arity> frame{};
    for (int i = 0; i < __arity; i++) {
      T sign = input[i] < T(0) ? T(-1) : T(1);
      T offset = threshold_ * sign;
      T result = input[i] * scaleAmount_ + offset;
      this->processed[i] = result;
    }

    return this->processed;
  }

  void threshold(T thresh) {
    thresh = fabs(thresh);
    thresh = std::min(thresh, signalMaxima);
    threshold_ = thresh;
    scaleAmount_ = signalMaxima - thresh;
  }

private:
  T threshold_;
  T scaleAmount_;
  T signalMaxima = T(1);
};

} // namespace Clover::DynamicRange
