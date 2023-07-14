#pragma once

#include <assert.h>
#include <cmath>

#include "Algo/AlgorithmBase.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::Envelope {

template <FloatingPoint T> struct Basic : public AlgorithmBase<T> {
  Basic() : Basic(0.f, 0.f, 1) {}

  Basic(float from, float to, uint numSteps) {
    set(from, to, numSteps);
    this->processed = from;
  }

  void set(float from, float to, int numSteps) {
    set(from, to, static_cast<uint>(numSteps));
  }

  void set(float from, float to, uint numSteps) {
    from_ = from;
    to_ = to;
    currentStep_ = 0;
    setCommon(numSteps);
  }

  void set(float to, int numSteps) { set(to, static_cast<uint>(numSteps)); }

  void set(float to, uint numSteps) {
    from_ = this->processed;
    to_ = to;

    // let the previous sample be the starting the point
    currentStep_ = 1;
    setCommon(numSteps);
  }

  T process() {

    if (currentStep_ >= targetStep_) {
      this->processed = to_;
      return to_;
    }

    T lerpAmount = currentStep_ * reciprocal_;
    assert(lerpAmount >= T(0) && lerpAmount <= T(1));

    this->processed = std::lerp(from_, to_, lerpAmount);

    currentStep_++;

    return this->processed;
  }

protected:
  T from_;
  T to_;
  T reciprocal_;
  uint currentStep_;
  uint targetStep_;

  void setCommon(uint numSteps) {
    numSteps = numSteps < 2 ? 2 : numSteps;

    reciprocal_ = 1 / static_cast<T>(numSteps - 1);
    targetStep_ = numSteps;
  }
};

} // namespace Clover::Envelope
