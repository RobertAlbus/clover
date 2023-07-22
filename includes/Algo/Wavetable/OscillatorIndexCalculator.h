#pragma once

#include <cmath>
#include <memory>
#include <vector>

#include "Algo/AlgorithmBase.h"
#include "OscillatorInterface.h"

#include "Util/FloatingPointConcept.h"

namespace Clover::Wavetable {

template <FloatingPoint T>
struct OscillatorIndexCalculator : public OscillatorInterface<T>,
                                   public AlgorithmBase<T> {
  OscillatorIndexCalculator(T sampleRateHz, int wavetableSize)
      : // clang-format off
      freq_(T(0)),
      freqReciprocal_(T(0)),
      readIndex_(T(0)),
      readIndexIncrementSamples_(T(0)),
      phaseOffsetPercent_(T(0)),
      phaseOffsetSamples_(T(0))
        // clang-format on
  {
    sampleRate(sampleRateHz);
    size(wavetableSize);
  }

  void freq(T freqHz) override {
    freqHz = normalizeFreq(freqHz);
    if (freq_ == freqHz)
      return;

    freq_ = freqHz;
    freqReciprocal_ = freq_ == T(0) ? T(0) : T(1) / freq_;

    readIndexIncrementSamples_ = calculateReadIndexIncrement(freq_);
  }

  T freq() override { return freq_; }

  void phase(T phasePercent) override {
    phasePercent = normalizePhase(phasePercent);
    readIndex_ =
        normalizeReadIndex(wavetableSize_ * phasePercent + phaseOffsetSamples_);
  }

  T phase() override {
    T deOffsetIndex = readIndex_ - phaseOffsetSamples_;

    if (deOffsetIndex < T(0))
      deOffsetIndex += wavetableSize_;

    return deOffsetIndex * wavetableSizeResciprocal_;
  }

  void phaseOffset(T offsetPercent) override {
    readIndex_ -= phaseOffsetSamples_;

    phaseOffsetPercent_ = normalizePhase(offsetPercent);
    phaseOffsetSamples_ = wavetableSize_ * phaseOffsetPercent_;

    readIndex_ += phaseOffsetSamples_;

    if (readIndex_ < T(0))
      readIndex_ += wavetableSize_;
  }

  T phaseOffset() override { return phaseOffsetPercent_; }

  T process() {
    this->processed = readIndex_;
    readIndex_ = normalizeReadIndex(readIndex_ + readIndexIncrementSamples_);

    return this->processed;
  }

  void period(T periodSamples) override { freq(sampleRate_ / periodSamples); }

  T period() override { return sampleRate_ * freqReciprocal_; }

  T sampleRate() override { return sampleRate_; }
  int size() override { return wavetableSize_; }

private:
  void size(int sizeSamples) {
    sizeSamples = normalizeSize(sizeSamples);
    wavetableSize_ = static_cast<T>(sizeSamples);
    wavetableSizeResciprocal_ =
        wavetableSize_ == T(0) ? T(0) : T(1) / wavetableSize_;
  }

  void sampleRate(T rateHz) {
    sampleRate_ = normalizeSampleRate(rateHz);
    sampleRateReciprocal_ = sampleRate_ == T(0) ? T(0) : T(1) / sampleRate_;
  }

  T normalizePhase(T phase) {
    phase = phase - static_cast<int>(phase);
    if (phase < T(0))
      phase += T(1);
    return phase;
  }

  T normalizeFreq(T freq) {
    if (freq < T(0))
      freq *= T(-1);
    return std::clamp(freq, T(0), nyquistThreshold());
  }

  T nyquistThreshold() { return sampleRate_ * T(0.5); }

  T normalizeSampleRate(T rateHz) { return std::max(T(0), rateHz); }

  T normalizeReadIndex(T index) {
    while (index >= wavetableSize_) {
      index -= wavetableSize_;
    }

    return index;
  }

  T calculateReadIndexIncrement(T freq) {
    return freq_ * wavetableSize_ * sampleRateReciprocal_;
  }

  int normalizeSize(int size) { return std::max(0, size); }

  T freq_;
  T freqReciprocal_;
  T readIndex_;
  T readIndexIncrementSamples_;

  T phaseOffsetPercent_;
  T phaseOffsetSamples_;

  T sampleRate_;
  T sampleRateReciprocal_;

  T wavetableSize_;
  T wavetableSizeResciprocal_;

public:
};

} // namespace Clover::Wavetable