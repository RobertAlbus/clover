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

#include "Clover/Base/CloverBase.h"

#include "Clover/Graph/AudioFrame.h"
#include "Clover/Graph/AudioNode.h"
#include "Clover/Graph/NullFrame.h"

#include "WavetableOsc.h"

#include "Clover/Algorithm/Wavetable/Generators.h"
#include "Clover/Algorithm/Wavetable/OscillatorInterface.h"

namespace Clover::Nodes::Wavetable {

WavetableOsc::WavetableOsc()
    : WavetableOsc(
          std::make_shared<std::vector<Sample>>(
              Clover::Wavetable::Sine<Sample>(256)
          ),
          200,
          0,
          0
      ) {}

WavetableOsc::WavetableOsc(
    std::shared_ptr<Wavetable> wavetable,
    float freq,
    float phase,
    float phaseOffset
)
    : Clover::Graph::AudioOutNode<1>(), osc(Base::sampleRate, wavetable) {}

void WavetableOsc::phase(float phasePercent) { osc.phase(phasePercent); }

float WavetableOsc::phase() { return osc.phase(); }

void WavetableOsc::phaseOffset(float offsetPercent) {
  osc.phaseOffset(offsetPercent);
}

float WavetableOsc::phaseOffset() { return osc.phaseOffset(); }

void WavetableOsc::freq(float freqHz) { osc.freq(freqHz); }

float WavetableOsc::freq() { return osc.freq(); }

void WavetableOsc::note(float midiNote) { osc.note(midiNote); }

float WavetableOsc::note() { return osc.note(); }

void WavetableOsc::period(float periodSamples) { osc.period(); }

float WavetableOsc::period() { return osc.period(); }

int WavetableOsc::size() { return osc.size(); }

float WavetableOsc::sampleRate() { return osc.sampleRate(); }

void WavetableOsc::wavetable(std::shared_ptr<Wavetable> wt) {
  osc.wavetable(wt);
}

std::shared_ptr<Wavetable> WavetableOsc::wavetable() { return osc.wavetable(); }

void WavetableOsc::sine(int size) { osc.sine(size); }
void WavetableOsc::square(int size) { osc.square(size); }
void WavetableOsc::saw(int size) { osc.saw(size); }
void WavetableOsc::tri(int size) { osc.tri(size); }
void WavetableOsc::noiseWhite(int size) { osc.noiseWhite(size); }

Graph::AudioFrame<1> WavetableOsc::tick(Graph::NullFrame input) {
  return Graph::AudioFrame<1>{osc.process()};
}

} // namespace Clover::Nodes::Wavetable
