#pragma once
#include <cmath>
#include <functional>
#include <memory>
#include <tgmath.h>
#include <vector>

#include "Algo/Wavetable/OscillatorInterface.h"
#include "Algo/Wavetable/WavetableOscillatorMono.h"
#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"
#include "WavetableOscInterface.h"

namespace Clover::NodeSimplex::Wavetable {

typedef std::vector<Sample> Wavetable;

struct WavetableOsc : public Base,
                      public Graph::AudioNode<0, 1>,
                      public Clover::Wavetable::WavetableOscInterface<float> {
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
  void period(float periodSamples) override;
  float period() override;

  int size() override;
  float sampleRate() override;

  void wavetable(std::shared_ptr<Wavetable> wt) override;
  std::shared_ptr<Wavetable> wavetable() override;

  void sine(int size = 512);
  void square(int size = 512);
  void saw(int size = 512);
  void tri(int size = 512);
  void noiseWhite(int size = 1024);

private:
  Graph::AudioFrame<1> tick(Graph::AudioFrame<0> input) override;
  Clover::Wavetable::WavetableOscillatorMono<Sample> osc;
};

} // namespace Clover::NodeSimplex::Wavetable