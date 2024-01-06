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
#include <iostream>

#include "Algorithm/AlgorithmBase.h"
#include "IIR_Coefficients.h"
#include "Util.h"

namespace Clover::Filter {

template <FloatingPoint T, int __arity>
class IIRFilterDF2T : AlgorithmBase<std::array<T, __arity>> {
public:
  IIRFilterDF2T() : inputStates_({{0}}), outputStates_({{0}}) {}

  IIRFilterDF2T(const IIRFilterCoefficients<T> &coeffs)
      : inputStates_({{0}}), outputStates_({{0}}), coeffs_(coeffs) {}

  void updateCoefficients(const IIRFilterCoefficients<T> &coeffs) {
    coeffs_ = coeffs;
    coeffs_.normalize();
  }

  std::array<T, __arity> process(const std::array<T, __arity> &input) {
    std::array<T, __arity> output;

    for (int ch = 0; ch < __arity; ++ch) {
      T yn = coeffs_.b0 * input[ch] + coeffs_.b1 * inputStates_[0][ch] +
             coeffs_.b2 * inputStates_[1][ch] -
             coeffs_.a1 * outputStates_[0][ch] -
             coeffs_.a2 * outputStates_[1][ch];

      inputStates_[1][ch] = inputStates_[0][ch];
      inputStates_[0][ch] = input[ch];
      outputStates_[1][ch] = outputStates_[0][ch];
      outputStates_[0][ch] = yn;

      output[ch] = yn;
    }

    this->processed = output;

    return output;
  }

private:
  IIRFilterCoefficients<T> coeffs_;
  std::array<std::array<T, __arity>, 2> inputStates_;
  std::array<std::array<T, __arity>, 2> outputStates_;
};

} // namespace Clover::Filter
