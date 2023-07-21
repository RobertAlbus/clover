#pragma once
#include <cmath>
#include <functional>
#include <memory>
#include <tgmath.h>
#include <vector>

#include "Util/FloatingPointConcept.h"

namespace Clover::Wavetable {

template <FloatingPoint T> using Wavetable = std::vector<T>;

template <FloatingPoint T> class OscillatorInterface {
public:
  virtual void phase(T phase) = 0;
  virtual T phase() = 0;
  virtual void phaseOffset(T offset) = 0;
  virtual T phaseOffset() = 0;

  virtual void freq(T freq) = 0;
  virtual T freq() = 0;
  virtual void period(T periodSamples) = 0;
  virtual T period() = 0;
  virtual int size() = 0;
  virtual T sampleRate() = 0;
};

template <FloatingPoint T> class WavetableOscillatorInterface {
public:
  virtual void wavetable(std::shared_ptr<Wavetable<T>> wt) = 0;
  virtual std::shared_ptr<Wavetable<T>> wavetable() = 0;
};

} // namespace Clover::Wavetable