#pragma once

#include <array>

#include "Algo/AlgorithmBase.h"
#include "Util/Calc.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::Stereo {

template <FloatingPoint T>
struct Difference : public AlgorithmBase<std::array<T, 2>> {

  Difference() {}

  std::array<T, 2> process(std::array<T, 2> input) {
    T sideL = (input[0] - input[1]) * T(0.5);
    T sideR = sideL * T(-1);

    this->processed[0] = sideL;
    this->processed[1] = sideR;

    return this->processed;
  }
};

} // namespace Clover::Stereo
