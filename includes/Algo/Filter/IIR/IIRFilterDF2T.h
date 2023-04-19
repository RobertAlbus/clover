#pragma once

#include <array>
#include <iostream>

#include "IIR_Coefficients.h"
#include "Util.h"

namespace Clover::Filter {

template <FloatingPoint T, int __arity> class IIRFilterDF2T {
public:
  IIRFilterDF2T() : inputStates_({{0}}), outputStates_({{0}}) {}

  IIRFilterDF2T(const IIRFilterCoefficients<T> &coeffs)
      : inputStates_({{0}}), outputStates_({{0}}), coeffs_(coeffs) {}

  void updateCoefficients(const IIRFilterCoefficients<T> &coeffs) {
    coeffs_ = coeffs;
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

    return output;
  }

private:
  IIRFilterCoefficients<T> coeffs_;
  std::array<std::array<T, __arity>, 2> inputStates_;
  std::array<std::array<T, __arity>, 2> outputStates_;
};

} // namespace Clover::Filter
