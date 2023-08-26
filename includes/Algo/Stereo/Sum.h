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

namespace Clover::Stereo {

template <FloatingPoint T> struct SumMono : public AlgorithmBase<T> {

  SumMono() {}

  T process(std::array<T, 2> input) {
    this->processed = (input[0] + input[1]) * T(0.5);

    return this->processed;
  }
};

template <FloatingPoint T>
struct SumStereo : public AlgorithmBase<std::array<T, 2>> {

  SumStereo() {}

  std::array<T, 2> process(std::array<T, 2> input) {
    this->processed[0] = (input[0] + input[1]) * T(0.5);
    this->processed[1] = this->processed[0];

    return this->processed;
  }
};

} // namespace Clover::Stereo
