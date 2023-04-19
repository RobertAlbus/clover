#pragma once

#include <array>
#include <cmath>
#include <functional>

#include "Algorithm.h"
#include "Constants.h"
#include "Graph.h"

namespace Clover::NodeSimplex::Filter {

template <size_t __arity> class EQ : public Node<__arity, __arity> {
public:
  EQ() : biquad(), sampleRate_(48000) {}

  void set(float f, float Q, float dbGain) {
    setFunction(f, Q, dbGain, sampleRate_);
  }

  void freq(float f) { setFunction(f, reso_, dbGain_, sampleRate_); }

  void reso(float Q) { setFunction(freq_, Q, dbGain_, sampleRate_); }

  void dbGain(float dbGain) { setFunction(freq_, reso_, dbGain, sampleRate_); }

  void lowShelf() {
    setFunction = [this](float freq, float reso, float dbGain,
                         float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      this->dbGain_ = dbGain;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy.lowShelf(freq, reso, dbGain, sampleRate);
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
          this->coefficientStrategy.highShelf(freq, reso, dbGain, sampleRate);
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
          this->coefficientStrategy.peakingEQ(freq, reso, dbGain, sampleRate);
      this->biquad.updateCoefficients(coefficients);
    };
  }

  Frame<__arity> tick(Frame<__arity> input) { return biquad.process(input); }

protected:
  float freq_;
  float reso_;
  float dbGain_;
  float sampleRate_;

  std::function<void(float, float, float, float)> setFunction;
  Clover::Filter::IIRFilterDF2T<Sample, __arity> biquad;

  // TODO: update this to be a type union that can be used for strategy
  // switching
  Clover::Filter::ResonantButterworthCoefficientStrategy<Sample>
      coefficientStrategy;
};

} // namespace Clover::NodeSimplex::Filter
