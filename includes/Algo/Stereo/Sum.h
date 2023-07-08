#pragma once

#include <array>

#include "Algo/AlgorithmBase.h"
#include "Util/Calc.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::Stereo {

template <FloatingPoint T> struct SumMono : public AlgorithmBase<T> {

  SumMono() {}

  T process(std::array<T, 2> input) {
    this->processed = (input[0] + input[1]) * T(0.5);

    return this->processed;
  }
};

template <FloatingPoint T>
struct SumStereo : public AlgorithmBase<std::array<T, 2>> {

  SumStereo() {}

  std::array<T, 2> process(std::array<T, 2> input) {
    this->processed[0] = (input[0] + input[1]) * T(0.5);
    this->processed[1] = this->processed[0];

    return this->processed;
  }
};

} // namespace Clover::Stereo
