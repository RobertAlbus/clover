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

#include "Algo/AlgorithmBase.h"
#include "Util/Calc.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::DynamicRange {

template <FloatingPoint T, int __arity>
struct AsymClip : public AlgorithmBase<std::array<T, __arity>> {

  AsymClip(float minimum = T(-1), float maximum = T(1))
      : minimum_(minimum), maximum_(maximum) {}

  std::array<T, __arity> process(std::array<T, __arity> input) {
    for (size_t i = 0; i < __arity; i++) {
      T sign = Util::Calc::sign(input[i]);
      if (sign < 0) {
        this->processed[i] = std::max(minimum_, input[i]);
      } else {
        this->processed[i] = std::min(maximum_, input[i]);
      }
    }
    return this->processed;
  }

  T min() { return minimum_; }
  void min(T minimum) { minimum_ = minimum; }

  T max() { return maximum_; }
  void max(T maximum) { maximum_ = maximum; }

private:
  T minimum_;
  T maximum_;
};

} // namespace Clover::DynamicRange
