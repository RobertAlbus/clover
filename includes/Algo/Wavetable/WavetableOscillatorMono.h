#pragma once

#include <memory>

#include "Algo/AlgorithmBase.h"
#include "Generators.h"
#include "OscillatorIndexCalculator.h"
#include "OscillatorInterface.h"

#include "Util/Calc.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::Wavetable {

template <FloatingPoint T>
struct WavetableOscillatorMono : public WavetableOscInterface<T>,
                                 public AlgorithmBase<T> {
  WavetableOscillatorMono(T sampleRateHz, Wavetable<T> wt = Sine<T>(512))
      : WavetableOscillatorMono(sampleRateHz,
                                std::make_shared<Wavetable<T>>(wt)) {}

  WavetableOscillatorMono(T sampleRateHz, std::shared_ptr<Wavetable<T>> wt)
      : calculator(sampleRateHz, wt->size()) {
    wavetable(wt);
  }

  void phase(T phase) override { calculator.phase(phase); }
  T phase() override { return calculator.phase(); }

  void phaseOffset(T offset) override { calculator.phaseOffset(offset); }
  T phaseOffset() override { return calculator.phaseOffset(); }

  void freq(T freq) override { calculator.freq(freq); }
  T freq() override { return calculator.freq(); }

  void period(T periodSamples) override { calculator.period(periodSamples); }
  T period() override { return calculator.period(); }

  int size() override { return calculator.size(); }
  T sampleRate() override { return calculator.sampleRate(); }

  void note(T midiNote) override {
    calculator.freq(Util::Calc::mtof(midiNote));
  }
  T note() override { return Util::Calc::mtof(calculator.freq()); }

  void wavetable(Wavetable<T> wt) {
    wavetable(std::make_shared<Wavetable<T>>(wt));
  }
  void wavetable(std::shared_ptr<Wavetable<T>> wt) override {
    wavetable_ = wt;

    if (calculator.size() == wt->size())
      return;

    calculator.size(wt->size());
  }
  std::shared_ptr<Wavetable<T>> wavetable() override { return wavetable_; }

  T process() {
    OscillatorIndexCalculatorResult<T> index = calculator.process();
    this->processed =
        std::lerp<T>((*wavetable_)[index.indexA], (*wavetable_)[index.indexB],
                     index.lerpAmount);
    return this->processed;
  }

  // clang-format off
  void sine      (int wavetableSize) { wavetable(Sine<T>       (wavetableSize)); }
  void square    (int wavetableSize) { wavetable(Square<T>     (wavetableSize)); }
  void saw       (int wavetableSize) { wavetable(Saw<T>        (wavetableSize)); }
  void tri       (int wavetableSize) { wavetable(Tri<T>        (wavetableSize)); }
  void noiseWhite(int wavetableSize) { wavetable(NoiseWhite<T> (wavetableSize)); }
  // clang-format on

private:
  OscillatorIndexCalculator<T> calculator;
  std::shared_ptr<Wavetable<T>> wavetable_;
};

} // namespace Clover::Wavetable