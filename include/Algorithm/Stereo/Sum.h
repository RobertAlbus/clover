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

#include "Util/FloatingPointConcept.h"

namespace Clover::Stereo {

template <FloatingPoint T> struct SumMono {

  SumMono() {}

  T process(std::array<T, 2> input) { return (input[0] + input[1]) * T(0.5); }
};

template <FloatingPoint T> struct SumStereo {

  SumStereo() {}

  std::array<T, 2> process(std::array<T, 2> input) {
    T sum = (input[0] + input[1]) * T(0.5);
    return {sum, sum};
  }
};

} // namespace Clover::Stereo
