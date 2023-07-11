#pragma once

#include <array>

#include "Algo/AlgorithmBase.h"
#include "Algo/Stereo/Difference.h"
#include "Algo/Stereo/Pan.h"
#include "Algo/Stereo/Sum.h"

#include "Util/FloatingPointConcept.h"

namespace Clover::Stereo {

template <FloatingPoint T>
struct MidSideBalance : public AlgorithmBase<std::array<T, 2>> {
  MidSideBalance(T s = T(0)) { spread(s); }

  T spread() { return spreadRaw_; }
  void spread(T s) {
    s = std::clamp(s, T(-1), T(1));
    spreadRaw_ = s;

    bool shouldNarrow = s < 0;
    if (shouldNarrow) {
      spreadCoefMid_ = T(1);
      spreadCoefSide_ = T(1) + s;
    } else {
      spreadCoefMid_ = T(1) - s;
      spreadCoefSide_ = T(1);
    }
  }

  std::array<T, 2> process(std::array<T, 2> input) {
    sum_.process(input);
    diff_.process(input);

    for (int i = 0; i < 2; i++) {
      T mid = sum_.last()[i] * spreadCoefMid_;
      T side = diff_.last()[i] * spreadCoefSide_;

      this->processed[i] = mid + side;
    }

    return this->processed;
  }

protected:
  Difference<T> diff_;
  SumStereo<T> sum_;

  T spreadRaw_;
  T spreadCoefMid_;
  T spreadCoefSide_;
};

} // namespace Clover::Stereo
