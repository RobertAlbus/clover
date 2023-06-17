#pragma once

#include <array>
#include <cmath>
#include <functional>

#include "Algorithm.h"
#include "Base.h"
#include "Graph.h"

#include "CoefficientStrategySettable.h"
#include "EQSettable.h"

namespace Clover::NodeSimplex::Filter {

template <size_t __arity>
class EQ : public EQSettable,
           public CoefficientStrategySettable,
           public Base,
           public AudioNode<__arity, __arity> {
public:
  EQ() : biquad() { butterworthResonant(); }

  void set(float f, float Q, float dbGain) {
    if (f == freq_ && Q == reso_ && dbGain == dbGain_)
      return;
    setFunction(f, Q, dbGain, Base::sampleRate);
  }

  float freq() { return freq_; }
  void freq(float f) {
    if (f == freq_)
      return;
    setFunction(f, reso_, dbGain_, Base::sampleRate);
  }

  float reso() { return reso_; }
  void reso(float Q) {
    if (Q == reso_)
      return;
    setFunction(freq_, Q, dbGain_, Base::sampleRate);
  }

  void dbGain(float dbGain) {
    if (dbGain == dbGain_)
      return;
    setFunction(freq_, reso_, dbGain, Base::sampleRate);
  }

  void lowShelf() {
    setFunction = [this](float freq, float reso, float dbGain,
                         float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      this->dbGain_ = dbGain;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy->lowShelf(freq, reso, dbGain, sampleRate);
      this->biquad.updateCoefficients(coefficients);
    };
  }

  void highShelf() {
    setFunction = [this](float freq, float reso, float dbGain,
                         float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      this->dbGain_ = dbGain;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy->highShelf(freq, reso, dbGain, sampleRate);
      this->biquad.updateCoefficients(coefficients);
    };
  }

  void peakingEQ() {
    setFunction = [this](float freq, float reso, float dbGain,
                         float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      this->dbGain_ = dbGain;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy->peakingEQ(freq, reso, dbGain, sampleRate);
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

  AudioFrame<__arity> tick(AudioFrame<__arity> input) { 
    input.data = biquad.process(input.data);
    return input;
  }
  
protected:
  float freq_;
  float reso_;
  float dbGain_;

  std::function<void(float, float, float, float)> setFunction;
  Clover::Filter::IIRFilterDF2T<Sample, __arity> biquad;

  std::unique_ptr<Clover::Filter::IIRCoefficientStrategy<Sample>>
      coefficientStrategy;

  void resetCoefficients() {
    peakingEQ();
    set(21000, 0.707, 0);
  }
};

} // namespace Clover::NodeSimplex::Filter
