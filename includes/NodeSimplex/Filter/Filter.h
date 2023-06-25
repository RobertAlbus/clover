#pragma once

#include <array>
#include <cmath>
#include <functional>

#include "Algorithm.h"
#include "Base.h"
#include "Graph.h"

#include "CoefficientStrategySettable.h"
#include "FilterSettable.h"

namespace Clover::NodeSimplex::Filter {

template <size_t __arity>
class Filter : public FilterSettable,
               public CoefficientStrategySettable,
               public Base,
               public AudioNode<__arity, __arity> {
public:
  Filter() : biquad() { butterworthResonant(); }

  void set(float f, float Q) override {
    if (f == freq_ && Q == reso_)
      return;
    setFunction(f, Q, Base::sampleRate);
  }

  float freq() override { return freq_; }
  void freq(float f) override {
    if (f == freq_)
      return;
    setFunction(f, reso_, Base::sampleRate);
  }

  float reso() override { return reso_; }
  void reso(float Q) override {
    if (Q == reso_)
      return;
    setFunction(freq_, Q, Base::sampleRate);
  }

  void lowPass() override {
    setFunction = [this](float freq, float reso, float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy->lowPass(freq, reso, sampleRate);
      this->biquad.updateCoefficients(coefficients);
    };
  }

  void highPass() override {
    setFunction = [this](float freq, float reso, float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy->highPass(freq, reso, sampleRate);
      this->biquad.updateCoefficients(coefficients);
    };
  }

  void notch() override {
    setFunction = [this](float freq, float reso, float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy->notch(freq, reso, sampleRate);
      this->biquad.updateCoefficients(coefficients);
    };
  }

  void bandPass() override {
    setFunction = [this](float freq, float reso, float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy->bandPass(freq, reso, sampleRate);
      this->biquad.updateCoefficients(coefficients);
    };
  }

  void butterworthResonant() override {
    coefficientStrategy = std::make_unique<
        Clover::Filter::ResonantButterworthCoefficientStrategy<Sample>>();
    resetCoefficients();
  }

  void butterworth() override {
    coefficientStrategy = std::make_unique<
        Clover::Filter::ButterworthCoefficientStrategy<Sample>>();
    resetCoefficients();
  }

  void chebyshevType1() override {
    coefficientStrategy = std::make_unique<
        Clover::Filter::ChebyshevType1CoefficientStrategy<Sample>>();
    resetCoefficients();
  }

  void chebyshevType2() override {
    coefficientStrategy = std::make_unique<
        Clover::Filter::ChebyshevType2CoefficientStrategy<Sample>>();
    resetCoefficients();
  }

  AudioFrame<__arity> tick(AudioFrame<__arity> input) {
    input.data = biquad.process(input.data);
    return input;
  }

protected:
  float freq_;
  float reso_;

  std::function<void(float, float, float)> setFunction;
  Clover::Filter::IIRFilterDF2T<Sample, __arity> biquad;

  std::unique_ptr<Clover::Filter::IIRCoefficientStrategy<Sample>>
      coefficientStrategy;

  void resetCoefficients() {
    lowPass();
    set(21000, 0.707);
  }
};

} // namespace Clover::NodeSimplex::Filter
