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

#include <functional>

#include "Clover/Algorithm/Filter_IIR.h"
#include "Clover/Base/CloverBase.h"
#include "Clover/Graph/AudioNode.h"

#include "FilterSettable.h"

namespace Clover::Nodes::Filter {

template <size_t __arity>
class Filter : public FilterSettable,
               public Base,
               public Graph::AudioNode<__arity, __arity> {
public:
  Filter()
      : biquad(),
        generateCoefficientsFunction(Clover::Filter::lowPass_rbj<float>),
        freq_(22000.f), reso_(0.707f) {}

  void set(float f, float Q) override {
    if (freq_ == f && reso_ == Q)
      return;
    freq_ = f;
    reso_ = Q;
    updateCoefficients();
  }

  float freq() override { return freq_; }
  void freq(float f) override {
    if (freq_ == f)
      return;
    freq_ = f;
    updateCoefficients();
  }

  float reso() override { return reso_; }
  void reso(float Q) override {
    if (reso_ == Q)
      return;
    reso_ = Q;
    updateCoefficients();
  }

  void lowPass() override {
    generateCoefficientsFunction = Clover::Filter::lowPass_rbj<float>;
    updateCoefficients();
  }

  void highPass() override {
    generateCoefficientsFunction = Clover::Filter::highPass_rbj<float>;
    updateCoefficients();
  }

  void notch() override {
    generateCoefficientsFunction = Clover::Filter::notch_rbj<float>;
    updateCoefficients();
  }

  void bandPass() override {
    generateCoefficientsFunction = Clover::Filter::bandPass_rbj<float>;
    updateCoefficients();
  }

  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    return biquad.process(input.data);
  }

protected:
  float freq_;
  float reso_;

  std::function<
      Clover::Filter::IIRFilterCoefficients<float>(float, float, float)>
      generateCoefficientsFunction;
  Clover::Filter::IIRFilterDF2T<float, __arity> biquad;

  void updateCoefficients() {
    auto coefficients = generateCoefficientsFunction(
        freq_, reso_, static_cast<float>(Base::sampleRate)
    );
    biquad.updateCoefficients(coefficients);
  }
};

} // namespace Clover::Nodes::Filter
