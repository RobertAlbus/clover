#include <cmath>
#include <functional>
#include <memory>
#include <tgmath.h>
#include <vector>

#include "Base.h"
#include "Graph.h"
#include "Util.h"

#include "WavetableOsc.h"
#include "Algo/Wavetable/Generators.h"
#include "Algo/Wavetable/OscillatorInterface.h"

namespace Clover::NodeSimplex::Wavetable {


WavetableOsc::WavetableOsc()
      : WavetableOsc(std::make_shared<std::vector<Sample>>(
                         Clover::Wavetable::Sine<Sample>(256)),
                     200, 0, 0) {}

WavetableOsc::WavetableOsc(std::shared_ptr<Wavetable> wavetable, float freq, float phase, float phaseOffset)
    : Clover::Graph::AudioNode<0, 1>(), osc(Base::sampleRate, wavetable) {
}

void WavetableOsc::phase(float phasePercent) {
  osc.phase(phasePercent);
}

float WavetableOsc::phase() {
  return osc.phase();
}

void WavetableOsc::phaseOffset(float offsetPercent) {
  osc.phaseOffset(offsetPercent);
}

float WavetableOsc::phaseOffset() {
  return osc.phaseOffset();
}

void WavetableOsc::freq(float freqHz) {
  osc.freq(freqHz);
}

float WavetableOsc::freq() {
  return osc.freq();
}

void WavetableOsc::note(float midiNote) {
  osc.note(midiNote);
}

float WavetableOsc::note() {
  return osc.note();
}

void WavetableOsc::period(float periodSamples) {
  osc.period();
}

float WavetableOsc::period() {
  return osc.period();
}

int WavetableOsc::size() {
  return osc.size();
}

float WavetableOsc::sampleRate() {
  return osc.sampleRate();
}

void WavetableOsc::wavetable(std::shared_ptr<Wavetable> wt) {
  osc.wavetable(wt);
}

std::shared_ptr<Wavetable> WavetableOsc::wavetable() {
  return osc.wavetable();
}

void WavetableOsc::sine(int size) {
  osc.sine(size);
}
void WavetableOsc::square(int size) {
  osc.square(size);
}
void WavetableOsc::saw(int size) {
  osc.saw(size);
}
void WavetableOsc::tri(int size) {
  osc.tri(size);
}
void WavetableOsc::noiseWhite(int size) {
  osc.noiseWhite(size);
}

Graph::AudioFrame<1> WavetableOsc::tick(Graph::AudioFrame<0> input) {
  return Graph::AudioFrame<1> { osc.process() };
}

} // namespace Clover::NodeSimplex::Wavetable