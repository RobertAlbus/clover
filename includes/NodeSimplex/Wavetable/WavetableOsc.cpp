#include <cmath>
#include <functional>
#include <memory>
#include <tgmath.h>
#include <vector>

#include "Base.h"
#include "Graph.h"
#include "Util.h"

#include "WavetableOsc.h"

namespace Clover::NodeSimplex::Wavetable {


WavetableOsc::WavetableOsc()
      : WavetableOsc(std::make_shared<std::vector<Sample>>(
                         Util::GenerateWavetable::Sine(256)),
                     200, 0, 0) {}

WavetableOsc::WavetableOsc(std::shared_ptr<Wavetable> wavetable, float freq, float phase, float phaseOffset)
    : Clover::Graph::StatefulProcessor<0, 1, WavetableOscSettings>(), WavetableOscInterface() {
  settings.initial.wavetable = wavetable;
  settings.initial.wavetableSize = (float)wavetable->size();
  settings.initial.freq = freq;
  settings.initial.phase = normalizePhase(phase);
  settings.initial.phaseOffset = normalizePhase(phaseOffset);

  settings.initial.readIndexIncrement = calculateReadIndexIncrement(freq);
  settings.initial.readIndex = 0.;
  settings.initial.readIndexOffset =
      calculateReadIndexOffset(settings.initial.phaseOffset);

  settings.reset();
}

void WavetableOsc::phase(float phase) {
  phase = normalizePhase(phase);
  settings.current.phase = phase;
  settings.current.readIndex = (settings.current.wavetableSize) * phase;
}

float WavetableOsc::phase() {
  return settings.current.phase / settings.current.wavetableSize;
}

void WavetableOsc::phaseOffset(float offset) {
  settings.current.phaseOffset = normalizePhase(offset);
  settings.current.readIndexOffset = calculateReadIndexOffset(offset);
}

float WavetableOsc::phaseOffset() { return settings.current.phaseOffset; }

void WavetableOsc::freq(float freq) {
  freq = fabs(freq);
  settings.current.freq = freq;
  settings.current.readIndexIncrement = calculateReadIndexIncrement(freq);
}

float WavetableOsc::freq() { return settings.current.freq; }

void WavetableOsc::note(float midiNote) { freq(Clover::Util::Calc::mtof(midiNote)); }

float WavetableOsc::note() { return Clover::Util::Calc::ftom(freq()); }

void WavetableOsc::wavelength(float wavelengthSamples) {
  freq(((float)Base::sampleRate) / wavelengthSamples);
}

float WavetableOsc::wavelength() { return ((float)Base::sampleRate) / freq(); }

void WavetableOsc::wavetable(std::shared_ptr<Wavetable> wt) {
  settings.current.wavetable = wt;
  settings.current.wavetableSize = (float)wt->size();
  settings.current.readIndexIncrement =
      calculateReadIndexIncrement(settings.current.freq);
}

std::shared_ptr<Wavetable> WavetableOsc::wavetable() { return settings.current.wavetable; }

void WavetableOsc::sine(int size) {
  std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
      Util::GenerateWavetable::Sine(size));
  wavetable(wt);
}
void WavetableOsc::square(int size) {
  std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
      Util::GenerateWavetable::Square(size));
  wavetable(wt);
}
void WavetableOsc::saw(int size) {
  std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
      Util::GenerateWavetable::Saw(size));
  wavetable(wt);
}
void WavetableOsc::tri(int size) {
  std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
      Util::GenerateWavetable::Tri(size));
  wavetable(wt);
}
void WavetableOsc::noise(int size) {
  std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
      Util::GenerateWavetable::NoiseWhite(size));
  wavetable(wt);
}

Frame<1> WavetableOsc::tick(Frame<0> input) {
  float direction = Clover::Util::Calc::sign(settings.current.freq);
  float nextIndex =
      normalizeReadIndex(floor(settings.current.readIndex) + direction);

  Wavetable &wt = *(settings.current.wavetable);

  float value =
      std::lerp(wt[(int)settings.current.readIndex], wt[(int)nextIndex],
                fmod(settings.current.readIndex, 1)
      );

  Frame<1> f{value};

  float newIndex = settings.current.readIndex +
                    settings.current.readIndexOffset +
                    settings.current.readIndexIncrement;

  settings.current.readIndex = normalizeReadIndex(newIndex);

  return f;
}

float WavetableOsc::normalizeReadIndex(float index) {
  float wtSize = (float)settings.current.wavetableSize;
  return fmod(fmod(index, wtSize) + wtSize, wtSize);
}

float WavetableOsc::normalizePhase(float phase) { return fmod(fmod(phase, 1.) + 1., 1); }

float WavetableOsc::calculateReadIndexIncrement(float freq) {
  return freq * ((float)settings.current.wavetableSize) /
          ((float)Base::sampleRate);
}

float WavetableOsc::calculateReadIndexOffset(float phaseOffset) {
  return ((float)settings.current.wavetableSize) *
          normalizePhase(phaseOffset);
}

void WavetableOsc::printSettings(WavetableOscSettings &settings) {
  printf("\n%f   - %f   - %f   - %f   - %f   - %f   - %f\n",
          settings.readIndex, settings.readIndexIncrement,
          settings.wavetableSize, settings.freq, settings.phase,
          settings.phaseOffset, settings.readIndexOffset);
}


} // namespace Clover::NodeSimplex::Wavetable