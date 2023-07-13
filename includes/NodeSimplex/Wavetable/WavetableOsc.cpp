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

namespace Clover::NodeSimplex::Wavetable {


WavetableOsc::WavetableOsc()
      : WavetableOsc(std::make_shared<std::vector<Sample>>(
                         Clover::Wavetable::Sine<Sample>(256)),
                     200, 0, 0) {}

WavetableOsc::WavetableOsc(std::shared_ptr<Wavetable> wavetable, float freq, float phase, float phaseOffset)
    : Clover::Graph::AudioNode<0, 1>(), WavetableOscInterface() {
  this->wavetable_ = wavetable;
  this->wavetableSize_ = (float)wavetable->size();
  this->freq_ = freq;
  this->phase_ = normalizePhase(phase);
  this->phasePreCalculationCache_ = this->phase_;
  this->phaseOffset_ = normalizePhase(phaseOffset);
  this->phaseOffsetPreCalculationCache_ = this->phaseOffset_;

  this->readIndexIncrement_ = calculateReadIndexIncrement(freq);
  this->readIndex_ = 0.;
  this->readIndexOffset_ =
      calculateReadIndexOffset(this->phaseOffset_);
}

void WavetableOsc::phase(float phase) {
  if (phase == this->phasePreCalculationCache_) return;
  phase = normalizePhase(phase);
  this->phase_ = phase;
  this->readIndex_ = (this->wavetableSize_) * phase;
}

float WavetableOsc::phase() {
  return this->phase_ / this->wavetableSize_;
}

void WavetableOsc::phaseOffset(float offset) {
  if (offset == this->phaseOffsetPreCalculationCache_) return;
  this->phaseOffset_ = normalizePhase(offset);
  this->readIndexOffset_ = calculateReadIndexOffset(offset);
}

float WavetableOsc::phaseOffset() { return this->phaseOffset_; }

void WavetableOsc::freq(float freq) {
  if (this->freq_ == freq || this->freq_ == freq * -1.f) return;

  freq = fabs(freq);
  this->freq_ = freq;
  this->readIndexIncrement_ = calculateReadIndexIncrement(freq); 
}

float WavetableOsc::freq() { return this->freq_; }

void WavetableOsc::note(float midiNote) { freq(Clover::Util::Calc::mtof(midiNote)); }

float WavetableOsc::note() { return Clover::Util::Calc::ftom(freq()); }

void WavetableOsc::wavelength(float wavelengthSamples) {
  freq(((float)Base::sampleRate) / wavelengthSamples);
}

float WavetableOsc::wavelength() { return ((float)Base::sampleRate) / freq(); }

void WavetableOsc::wavetable(std::shared_ptr<Wavetable> wt) {
  this->wavetable_ = wt;
  this->wavetableSize_ = (float)wt->size();
  this->readIndexIncrement_ =
      calculateReadIndexIncrement(this->freq_);
}

std::shared_ptr<Wavetable> WavetableOsc::wavetable() { return this->wavetable_; }

void WavetableOsc::sine(int size) {
  std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
      Clover::Wavetable::Sine<Sample>(size));
  wavetable(wt);
}
void WavetableOsc::square(int size) {
  std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
      Clover::Wavetable::Square<Sample>(size));
  wavetable(wt);
}
void WavetableOsc::saw(int size) {
  std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
      Clover::Wavetable::Saw<Sample>(size));
  wavetable(wt);
}
void WavetableOsc::tri(int size) {
  std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
      Clover::Wavetable::Tri<Sample>(size));
  wavetable(wt);
}
void WavetableOsc::noise(int size) {
  std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
      Clover::Wavetable::NoiseWhite<Sample>(size));
  wavetable(wt);
}

Graph::AudioFrame<1> WavetableOsc::tick(Graph::AudioFrame<0> input) {
  float nextIndex =
      normalizeReadIndex(floor(this->readIndex_ + 1));

  Wavetable &wt = *(this->wavetable_);

  float lerpAmount = this->readIndex_ - static_cast<int>(this->readIndex_);
  float value =
      std::lerp(wt[(int)this->readIndex_], wt[(int)nextIndex], lerpAmount
      );

  Graph::AudioFrame<1> f{value};

  float newIndex = this->readIndex_ +
                    this->readIndexOffset_ +
                    this->readIndexIncrement_;

  this->readIndex_ = normalizeReadIndex(newIndex);

  return f;
}

float WavetableOsc::normalizeReadIndex(float index) {
  while (index > this->wavetableSize_) {
    index -= this->wavetableSize_;
  }
  return index;
}

float WavetableOsc::normalizePhase(float phase) {
  phase = phase - static_cast<int>(phase);
  if (phase < 0) phase += 1;
  // while (phase > 1.f) {
  //   phase -= 1.f;
  // }
  // while (phase < 0.f) {
  //   phase += 1.f;
  // }

  return phase;
}

float WavetableOsc::calculateReadIndexIncrement(float freq) {
  return freq * this->wavetableSize_ / Base::sampleRate;
}

float WavetableOsc::calculateReadIndexOffset(float phaseOffset) {
  return this->wavetableSize_ * normalizePhase(phaseOffset);
}

void WavetableOsc::printSettings() {
  printf("\n%f   - %f   - %f   - %f   - %f   - %f   - %f\n",
          this->readIndex_, this->readIndexIncrement_,
          this->wavetableSize_, this->freq_, this->phase_,
          this->phaseOffset_, this->readIndexOffset_);
}


} // namespace Clover::NodeSimplex::Wavetable