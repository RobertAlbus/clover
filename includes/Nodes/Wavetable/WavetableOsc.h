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
#include <memory>
#include <tgmath.h>
#include <vector>

#include "Algo/Wavetable/OscillatorInterface.h"
#include "Algo/Wavetable/WavetableOscillatorMono.h"
#include "Base/CloverBase.h"
#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"
#include "Graph/NullFrame.h"
#include "WavetableOscInterface.h"

namespace Clover::Nodes::Wavetable {

typedef std::vector<Sample> Wavetable;

struct WavetableOsc : public Graph::AudioOutNode<1>,
                      public Base,
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
  Graph::AudioFrame<1> tick(Graph::NullFrame input) override;
  Clover::Wavetable::WavetableOscillatorMono<Sample> osc;
};

} // namespace Clover::Nodes::Wavetable
