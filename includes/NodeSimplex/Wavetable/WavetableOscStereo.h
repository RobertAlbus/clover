#pragma once

#include <cmath>
#include <functional>
#include <tgmath.h>
#include <vector>

#include "NodeSimplex.h"
#include "WavetableOsc.h"

namespace Clover::NodeSimplex::Wavetable {

class WavetableOscStereo : public Base,
                           public Graph::AudioNode<0, 2>,
                           public WavetableOscInterface {
public:
  WavetableOscStereo();

  // detune from each other
  void stereoDetune(float semitones);
  float stereoDetune();

  void phase(float phase) override;
  float phase() override;
  void phaseOffset(float offset) override;
  float phaseOffset() override;

  void freq(float freq) override;
  float freq() override;
  void note(float midiNote) override;
  float note() override;
  void wavelength(float wavelengthSamples) override;
  float wavelength() override;

  void wavetable(std::shared_ptr<Wavetable> wt) override;
  std::shared_ptr<Wavetable> wavetable() override;

  void sine(int size = 512) override;
  void square(int size = 512) override;
  void saw(int size = 512) override;
  void tri(int size = 512) override;
  void noise(int size = 1024) override;

private:
  WavetableOsc oscL;
  WavetableOsc oscR;
  Adapter::NullAdapter<1, 0> blackHole;

  float stereoDetune_L_semi;
  float stereoDetune_R_semi;
  float stereoDetuneSemitones; // used for a quick pre-calculation check

  float freq_;

  Graph::AudioFrame<2> tick(Graph::AudioFrame<0> input);

  void connectNodes();
  void updateFreq();
};

} // namespace Clover::NodeSimplex::Wavetable