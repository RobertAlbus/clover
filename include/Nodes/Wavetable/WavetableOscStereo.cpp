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

#include "Algorithm/Math.h"
#include "Constants.h"
#include "Graph.h"
#include "Nodes.h"
#include "Util.h"

#include "WavetableOscStereo.h"

namespace Clover::Nodes::Wavetable {

WavetableOscStereo::WavetableOscStereo()
    : stereoDetune_L_semi(0), stereoDetune_R_semi(0), freq_(0) {
  connectNodes();
}

// detune from each other
void WavetableOscStereo::stereoDetune(float semitones) {
  if (semitones == stereoDetuneSemitones)
    return;

  stereoDetuneSemitones = semitones;
  stereoDetune_R_semi = 0.5 * semitones;
  stereoDetune_L_semi = stereoDetune_R_semi * -1;

  updateFreq();
}

float WavetableOscStereo::stereoDetune() { return stereoDetune_L_semi * 2; }

void WavetableOscStereo::phase(float phase) {
  oscL.phase(phase);
  oscR.phase(phase);
}

float WavetableOscStereo::phase() {
  return 0.f; // how determine phase of two detuned oscs? bad time.
}

void WavetableOscStereo::phaseOffset(float offset) {
  oscL.phaseOffset(offset);
  oscR.phaseOffset(offset);
}

float WavetableOscStereo::phaseOffset() { return oscL.phaseOffset(); }

void WavetableOscStereo::freq(float freq) {
  if (freq == freq_)
    return;
  freq_ = freq;
  updateFreq();
}

void WavetableOscStereo::updateFreq() {
  oscL.freq(
      Algorithm::frequency_by_semitone_difference(freq_, stereoDetune_L_semi)
  );
  oscR.freq(
      Algorithm::frequency_by_semitone_difference(freq_, stereoDetune_R_semi)
  );
}

float WavetableOscStereo::freq() { return freq_; }

void WavetableOscStereo::note(float midiNote) {
  freq(Util::Calc::mtof(midiNote));
}

float WavetableOscStereo::note() { return Util::Calc::ftom(freq()); }

void WavetableOscStereo::wavelength(float wavelengthSamples) {
  freq(((float)Base::sampleRate) / wavelengthSamples);
}

float WavetableOscStereo::wavelength() {
  return ((float)Base::sampleRate) / freq();
}

void WavetableOscStereo::wavetable(std::shared_ptr<Wavetable> wt) {
  oscL.wavetable(wt);
  oscR.wavetable(wt);
}

std::shared_ptr<Wavetable> WavetableOscStereo::wavetable() {
  return oscL.wavetable();
}

void WavetableOscStereo::sine(int size) {
  oscL.sine(size);
  oscR.wavetable(oscL.wavetable());
}
void WavetableOscStereo::square(int size) {
  oscL.square(size);
  oscR.wavetable(oscL.wavetable());
}
void WavetableOscStereo::saw(int size) {
  oscL.saw(size);
  oscR.wavetable(oscL.wavetable());
}
void WavetableOscStereo::tri(int size) {
  oscL.tri(size);
  oscR.wavetable(oscL.wavetable());
}
void WavetableOscStereo::noise(int size) {
  oscL.noiseWhite(size);
  oscR.wavetable(oscL.wavetable());
}

Graph::AudioFrame<2> WavetableOscStereo::tick(Graph::NullFrame input) {
  return Graph::AudioFrame<2>{oscL.frames.current[0], oscR.frames.current[0]};
}

void WavetableOscStereo::connectNodes() {
  oscL >> blackHole >> this;
  oscR >> blackHole;
}

} // namespace Clover::Nodes::Wavetable
