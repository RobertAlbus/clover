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

#include "Algorithm/AlgorithmBase.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::DynamicRange {

template <FloatingPoint T, int __arity>
struct UpwardDCCompression : public AlgorithmBase<std::array<T, __arity>> {

  UpwardDCCompression(T threshold = 0.f) { this->threshold(threshold); }

  std::array<T, __arity> process(std::array<T, __arity> input) {
    std::array<T, __arity> frame{};
    for (size_t i = 0; i < __arity; i++) {
      T sign = input[i] < T(0) ? T(-1) : T(1);
      T offset = threshold_ * sign;
      T result = input[i] * scaleAmount_ + offset;
      this->processed[i] = result;
    }

    return this->processed;
  }

  void threshold(T thresh) {
    thresh = fabs(thresh);
    thresh = std::min(thresh, signalMaxima);
    threshold_ = thresh;
    scaleAmount_ = signalMaxima - thresh;
  }

private:
  T threshold_;
  T scaleAmount_;
  T signalMaxima = T(1);
};

} // namespace Clover::DynamicRange
