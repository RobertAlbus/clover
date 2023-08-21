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
#include <vector>

#include "Util.h"

template <FloatingPoint T, int __arity> class FIRFilter {
public:
  FIRFilter(const std::vector<T> &coeffs)
      : coeffs_(coeffs), inputs_(coeffs.size(), T(0)) {}

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
