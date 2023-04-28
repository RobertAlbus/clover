#pragma once

#include <array>
#include <cmath>
#include <functional>

#include "Algorithm.h"
#include "Constants.h"
#include "Graph.h"

namespace Clover::NodeSimplex::Filter {

template <size_t __arity> class Filter : public Node<__arity, __arity> {
public:
  Filter() : biquad(), sampleRate_(48000) {
    coefficientStrategy = std::make_unique<
        Clover::Filter::ResonantButterworthCoefficientStrategy<Sample>>();
    resetCoefficients();
  }

  void set(float f, float Q) { setFunction(f, Q, sampleRate_); }

  void freq(float f) { setFunction(f, reso_, sampleRate_); }

  void reso(float Q) { setFunction(freq_, Q, sampleRate_); }

  void lowPass() {
    setFunction = [this](float freq, float reso, float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy->lowPass(freq, reso, sampleRate);
      this->biquad.updateCoefficients(coefficients);
    };
  }

  void highPass() {
    setFunction = [this](float freq, float reso, float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy->highPass(freq, reso, sampleRate);
      this->biquad.updateCoefficients(coefficients);
    };
  }

  void notch() {
    setFunction = [this](float freq, float reso, float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy->notch(freq, reso, sampleRate);
      this->biquad.updateCoefficients(coefficients);
    };
  }

  void bandPass() {
    setFunction = [this](float freq, float reso, float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy->bandPass(freq, reso, sampleRate);
      this->biquad.updateCoefficients(coefficients);
    };
  }

  void butterworthResonant() {
    coefficientStrategy = std::make_unique<
        Clover::Filter::ResonantButterworthCoefficientStrategy<Sample>>();
    resetCoefficients();
  }

  void butterworth() {
    coefficientStrategy = std::make_unique<
        Clover::Filter::ButterworthCoefficientStrategy<Sample>>();
    resetCoefficients();
  }

  void chebyshevType1() {
    coefficientStrategy = std::make_unique<
        Clover::Filter::ChebyshevType1CoefficientStrategy<Sample>>();
    resetCoefficients();
  }

  void chebyshevType2() {
    coefficientStrategy = std::make_unique<
        Clover::Filter::ChebyshevType2CoefficientStrategy<Sample>>();
    resetCoefficients();
  }

  Frame<__arity> tick(Frame<__arity> input) { return biquad.process(input); }

protected:
  float freq_;
  float reso_;
  float sampleRate_;

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
