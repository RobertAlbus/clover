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
#include <memory>

#include "Algorithm.h"
#include "Base.h"
#include "Graph.h"

#include "CoefficientStrategySettable.h"
#include "EQSettable.h"

namespace Clover::Nodes::Filter {

template <size_t __arity>
class EQ : public EQSettable,
           public CoefficientStrategySettable,
           public Base,
           public Graph::AudioNode<__arity, __arity> {
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
    setFunction =
        [this](float freq, float reso, float dbGain, float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      this->dbGain_ = dbGain;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy->lowShelf(freq, reso, dbGain, sampleRate);
      this->biquad.updateCoefficients(coefficients);
    };
  }

  void highShelf() {
    setFunction =
        [this](float freq, float reso, float dbGain, float sampleRate) {
      this->freq_ = freq;
      this->reso_ = reso;
      this->dbGain_ = dbGain;
      IIRFilterCoefficients<Sample> coefficients =
          this->coefficientStrategy->highShelf(freq, reso, dbGain, sampleRate);
      this->biquad.updateCoefficients(coefficients);
    };
  }

  void peakingEQ() {
    setFunction =
        [this](float freq, float reso, float dbGain, float sampleRate) {
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
        Clover::Filter::RbjBiquadCoefficientStrategy<Sample>>();
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

  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
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

} // namespace Clover::Nodes::Filter
