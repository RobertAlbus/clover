#pragma once

#include "Algo/AlgorithmBase.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::Envelope {

template <FloatingPoint T> struct DC : public AlgorithmBase<T> {

  DC(float value = 0.f) : value_(value) {}

  T process() {
    this->processed = value_;
    return value_;
  }

  void value(T v) { value_ = v; }

private:
  T value_;
};

} // namespace Clover::Envelope
