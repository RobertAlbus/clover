#pragma once

#include <cmath>

#include "IIR_Coefficients.h"
#include "Util.h"

namespace Clover::Filter {

template <FloatingPoint T> struct IIRCoefficientStrategy {
  // clang-format off
  virtual IIRFilterCoefficients<T> lowPass(T cutoff_hz, T Q, T samplerate_hz) = 0;
  virtual IIRFilterCoefficients<T> highPass(T cutoff_hz, T Q, T samplerate_hz) = 0;
  virtual IIRFilterCoefficients<T> bandPass(T cutoff_hz, T Q, T samplerate_hz) = 0;
  virtual IIRFilterCoefficients<T> notch(T cutoff_hz, T Q, T samplerate_hz) = 0;
  virtual IIRFilterCoefficients<T> lowShelf(T cutoff_hz, T Q, T gain_db, T samplerate_hz) = 0;
  virtual IIRFilterCoefficients<T> highShelf(T cutoff_hz, T Q, T gain_db, T samplerate_hz) = 0;
  virtual IIRFilterCoefficients<T> peakingEQ(T center_freq_hz, T Q, T gain_db, T samplerate_hz) = 0;
  // clang-format on
};

} // namespace Clover::Filter
