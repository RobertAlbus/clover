#pragma once
#include <cmath>
#include <functional>
#include <memory>
#include <tgmath.h>
#include <vector>

#include "WavetableOscInterface.h"

namespace Clover::NodeSimplex::Wavetable {

typedef std::vector<Sample> Wavetable;

struct WavetableOscSettings {
  float freq;
  float phase;
  float phasePreCalculationCache;
  float phaseOffset;
  float phaseOffsetPreCalculationCache;
  float readIndex;
  float readIndexIncrement;
  float readIndexOffset;
  std::shared_ptr<Wavetable> wavetable;
  float wavetableSize;
};

struct WavetableOsc
    : public Base,
      public Graph::StatefulProcessor<0, 1, WavetableOscSettings>,
      public WavetableOscInterface {
  WavetableOsc();
  WavetableOsc(std::shared_ptr<Wavetable> wavetable, float freq,
               float phase = 0, float phaseOffset = 0);

  void phase(float phase) override;
  float phase() override;
  void phaseOffset(float offset) override;
  float phaseOffset() override;

  void freq(float freq) override;
  float freq() override;
  virtual void note(float midiNote) override;
  virtual float note() override;
  void wavelength(float wavelengthSamples) override;
  float wavelength() override;

  void wavetable(std::shared_ptr<Wavetable> wt) override;
  std::shared_ptr<Wavetable> wavetable() override;

  void sine(int size = 512);
  void square(int size = 512);
  void saw(int size = 512);
  void tri(int size = 512);
  void noise(int size = 1024);

private:
  Graph::AudioFrame<1> tick(Graph::AudioFrame<0> input) override;

  float normalizeReadIndex(float index);
  float normalizePhase(float phase);
  float calculateReadIndexIncrement(float freq);
  float calculateReadIndexOffset(float phaseOffset);

public:
  void printSettings(WavetableOscSettings &settings);
};

} // namespace Clover::NodeSimplex::Wavetable