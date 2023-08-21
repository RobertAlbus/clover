#pragma once

/*
 * /////////
 * // Clover
 * 
 * Audio processing algorithms and DAG with feedback loops that do not break
 * acyclicity.
 * 
 * Copyright (C) 2023 Rob W. Albus
 *  
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 */


#include <cmath>
#include <functional>
#include <tgmath.h>
#include <vector>

#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"
#include "Graph/NullFrame.h"
#include "NodeSimplex.h"
#include "WavetableOsc.h"

namespace Clover::NodeSimplex::Wavetable {

class WavetableOscStereo : public Base,
                           public Graph::AudioOutNode<2>,
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
  Adapter::NullOutAdapter<1> blackHole;

  float stereoDetune_L_semi;
  float stereoDetune_R_semi;
  float stereoDetuneSemitones; // used for a quick pre-calculation check

  float freq_;

  Graph::AudioFrame<2> tick(Graph::NullFrame input);

  void connectNodes();
  void updateFreq();
};

} // namespace Clover::NodeSimplex::Wavetable
