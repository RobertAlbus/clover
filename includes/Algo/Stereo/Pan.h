#pragma once

#include <algorithm>

#include "Algo/AlgorithmBase.h"
#include "Util/Calc.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::Stereo {

template <FloatingPoint T> struct PanBase {
  PanBase(T p = T(0)) : midGain_(T(Util::Calc::dbtol(-4.5))) { pan(p); }

  T pan() { return pan_; }
  void pan(T p) {
    pan_ = std::clamp(p, T(-1), T(1));

    p = fabs(pan_);
    T panDown = std::lerp(midGain_, T(0), p);
    T panUp = std::lerp(midGain_, T(1), p);

    bool isPannedLeft = pan_ < 0;
    coefL_ = isPannedLeft ? panUp : panDown;
    coefR_ = isPannedLeft ? panDown : panUp;
  }

protected:
  T pan_;
  T midGain_;
  T coefL_;
  T coefR_;
};

template <FloatingPoint T>
struct PanMono : public PanBase<T>, public AlgorithmBase<std::array<T, 2>> {

  PanMono(T pan = T(0)) : PanBase<T>(pan) {}

  std::array<T, 2> process(T input) {
    this->processed[0] = input * this->coefL_;
    this->processed[1] = input * this->coefR_;

    return this->processed;
  }
};

template <FloatingPoint T>
struct PanStereo : public PanBase<T>, public AlgorithmBase<std::array<T, 2>> {

  PanStereo(T pan = T(0)) : PanBase<T>(pan) {}

  std::array<T, 2> process(std::array<T, 2> input) {
    this->processed[0] = input[0] * this->coefL_;
    this->processed[1] = input[1] * this->coefR_;

    return this->processed;
  }
};

} // namespace Clover::Stereo
