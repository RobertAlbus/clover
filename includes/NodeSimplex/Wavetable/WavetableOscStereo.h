#pragma once
#include <cmath>
#include <functional>
#include <tgmath.h>
#include <vector>

#include "Constants.h"
#include "Graph.h"
#include "NodeSimplex.h"
#include "Util.h"
#include "WavetableOsc.h"

namespace Clover::NodeSimplex::Wavetable {

class WavetableOscStereo : public Node<0, 2> {
public:
  WavetableOscStereo()
      : stereoDetune_L_semi(0), stereoDetune_R_semi(0), freq_(0) {
    connectNodes();
  }

  // detune from each other
  void stereoDetune(float semitones) {
    stereoDetune_R_semi = 0.5 * semitones;
    stereoDetune_L_semi = stereoDetune_R_semi * -1;

    freq(freq_);
  }

  float stereoDetune() { return stereoDetune_L_semi * 2; }

  void phase(float phase) {
    oscL.phase(phase);
    oscR.phase(phase);
  }

  void phaseOffset(float offset) {
    oscL.phaseOffset(offset);
    oscR.phaseOffset(offset);
  }

  float phaseOffset() { return oscL.phaseOffset(); }

  void freq(float freq) {
    freq_ = freq;
    oscL.freq(freq * powf(2, (stereoDetune_L_semi / 12)));
    oscR.freq(freq * powf(2, (stereoDetune_R_semi / 12)));
  }

  float freq() { return freq_; }

  void note(float midiNote) { freq(Clover::Util::Calc::mtof(midiNote)); }

  float note() { return Clover::Util::Calc::ftom(freq()); }

  void wavetable(std::shared_ptr<Wavetable> wt) {
    oscL.wavetable(wt);
    oscR.wavetable(wt);
  }

  std::shared_ptr<Wavetable> wavetable() { return oscL.wavetable(); }

  void sine(int size = 512) {
    oscL.sine(size);
    oscR.wavetable(oscL.wavetable());
  }
  void square(int size = 512) {
    oscL.square(size);
    oscR.wavetable(oscL.wavetable());
  }
  void saw(int size = 512) {
    oscL.saw(size);
    oscR.wavetable(oscL.wavetable());
  }
  void tri(int size = 512) {
    oscL.tri(size);
    oscR.wavetable(oscL.wavetable());
  }
  void noise(int size = 1024) {
    oscL.noise(size);
    oscR.wavetable(oscL.wavetable());
  }

private:
  WavetableOsc oscL;
  WavetableOsc oscR;
  Adapter::NullAdapter<1, 0> blackHole;

  float stereoDetune_L_semi;
  float stereoDetune_R_semi;

  float freq_;

  Frame<2> tick(Frame<0> input) {
    return Frame<2>{oscL.frames.current[0], oscR.frames.current[0]};
  }

  void connectNodes() {
    oscL >> blackHole >> this;
    oscR >> blackHole;
  }
};

} // namespace Clover::NodeSimplex::Wavetable