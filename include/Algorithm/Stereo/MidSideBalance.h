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

#include "Algorithm/Stereo/Difference.h"
#include "Algorithm/Stereo/Pan.h"
#include "Algorithm/Stereo/Sum.h"

#include "Util/FloatingPointConcept.h"

namespace Clover::Stereo {

template <FloatingPoint T> struct MidSideBalance {
  MidSideBalance(T s = T(0)) { spread(s); }

  T spread() { return spreadRaw_; }
  void spread(T s) {
    s = std::clamp(s, T(-1), T(1));
    spreadRaw_ = s;

    bool shouldNarrow = s < 0;
    if (shouldNarrow) {
      // I think this is wrong...
      spreadCoefMid_ = T(1);
      spreadCoefSide_ = T(1) + s;
    } else {
      spreadCoefMid_ = T(1) - s;
      spreadCoefSide_ = T(1);
    }
  }

  std::array<T, 2> process(std::array<T, 2> input) {
    std::array<T, 2> mid = sum_.process(input);
    std::array<T, 2> side = diff_.process(input);

    std::array<T, 2> output{};
    for (int i = 0; i < 2; i++) {
      output[i] = mid[i] * spreadCoefMid_ + side[i] * spreadCoefSide_;
    }

    return output;
  }

protected:
  Difference<T> diff_;
  SumStereo<T> sum_;

  T spreadRaw_;
  T spreadCoefMid_;
  T spreadCoefSide_;
};

} // namespace Clover::Stereo
