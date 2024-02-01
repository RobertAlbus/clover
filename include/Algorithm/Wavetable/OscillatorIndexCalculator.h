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

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "OscillatorInterface.h"

#include "Util/FloatingPointConcept.h"

namespace Clover::Wavetable {

template <FloatingPoint T> struct OscillatorIndexCalculatorResult {
  int indexA;
  int indexB;
  T lerpAmount;
};

template <FloatingPoint T>
struct OscillatorIndexCalculator
    : public OscillatorIndexCalculculatorInterface<T> {

  OscillatorIndexCalculator(T sampleRateHz, int wavetableSize = 0)
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

    calculateReadIndexIncrement();
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

  OscillatorIndexCalculatorResult<T> process() {
    OscillatorIndexCalculatorResult<T> result{};
    result.indexA = static_cast<int>(readIndex_);
    result.indexB = static_cast<int>(normalizeReadIndex(result.indexA + 1));
    result.lerpAmount = getFractionalComponent(readIndex_);
    readIndex_ = normalizeReadIndex(readIndex_ + readIndexIncrementSamples_);

    return result;
  }

  void period(T periodSamples) override { freq(sampleRate_ / periodSamples); }

  T period() override { return sampleRate_ * freqReciprocal_; }

  T sampleRate() override { return sampleRate_; }
  int size() override { return wavetableSize_; }
  void size(int sizeSamples) {
    sizeSamples = normalizeSize(sizeSamples);
    wavetableSize_ = static_cast<T>(sizeSamples);
    wavetableSizeResciprocal_ =
        wavetableSize_ == T(0) ? T(0) : T(1) / wavetableSize_;
    calculateReadIndexIncrement();
  }

private:
  void sampleRate(T rateHz) {
    sampleRate_ = normalizeSampleRate(rateHz);
    sampleRateReciprocal_ = sampleRate_ == T(0) ? T(0) : T(1) / sampleRate_;
  }

  T normalizePhase(T phase) { return getFractionalComponent(phase); }

  T getFractionalComponent(T num) {
    num = num - static_cast<int>(num);
    if (num < T(0))
      num += T(1);
    return num;
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

  void calculateReadIndexIncrement() {
    readIndexIncrementSamples_ = freq_ * wavetableSize_ * sampleRateReciprocal_;
  }

  int normalizeSize(int size) { return std::max(0, size); }

  OscillatorIndexCalculatorResult<T> previous;

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
};

} // namespace Clover::Wavetable
