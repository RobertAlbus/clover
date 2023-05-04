#pragma once
#include <cmath>
#include <functional>
#include <memory>
#include <tgmath.h>
#include <vector>

#include "Constants.h"
#include "Util.h"

namespace Clover::NodeSimplex::Wavetable {

typedef std::vector<Sample> Wavetable;

class WavetableOscInterface {
public:
  virtual void phase(float phase) = 0;
  virtual float phase() = 0;
  virtual void phaseOffset(float offset) = 0;
  virtual float phaseOffset() = 0;

  virtual void freq(float freq) = 0;
  virtual float freq() = 0;
  virtual void note(float midiNote) = 0;
  virtual float note() = 0;
  virtual void wavelength(float wavelengthSamples) = 0;
  virtual float wavelength() = 0;

  virtual void wavetable(std::shared_ptr<Wavetable> wt) = 0;
  virtual std::shared_ptr<Wavetable> wavetable() = 0;

  virtual void sine(int size = 512) = 0;
  virtual void square(int size = 512) = 0;
  virtual void saw(int size = 512) = 0;
  virtual void tri(int size = 512) = 0;
  virtual void noise(int size = 1024) = 0;
};

} // namespace Clover::NodeSimplex::Wavetable