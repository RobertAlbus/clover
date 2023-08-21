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
               public Graph::AudioNode<__arity, __arity> {
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

  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
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
