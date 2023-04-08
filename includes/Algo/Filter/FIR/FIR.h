#pragma once

#include <array>
#include <vector>

#include "Util.h"

template <FloatingPoint T, int __arity> class FIRFilter {
public:
  FIRFilter(const std::vector<T> &coeffs) : coeffs_(coeffs), inputs_(coeffs.size(), T(0)) {}

  std::array<T, __arity> tick(const std::array<T, __arity> &input) {
    inputs_.write(input);
    for (int channel = 0; channel < __arity; channel++) {
      for (int i = 0; i < coeffs_.size(); i++) {
        output[channel] += coeffs_[i] * inputs_[i][channel];
      }
    }
    return output;
  }

private:
  std::vector<T> coeffs_;
  HistoryBuffer<std::array<T, __arity>> inputs_;
};
