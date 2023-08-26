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

#include <algorithm>

#include "Algo/AlgorithmBase.h"
#include "Util/Calc.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::Stereo {

template <FloatingPoint T> struct PanBase {
  PanBase(T p = T(0)) : midGain_(T(Util::Calc::dbtol(-4.5))) { pan(p); }

  T pan() { return pan_; }
  void pan(T p) {
    pan_ = std::clamp(p, T(-1), T(1));

    p = fabs(pan_);
    T panDown = std::lerp(midGain_, T(0), p);
    T panUp = std::lerp(midGain_, T(1), p);

    bool isPannedLeft = pan_ < 0;
    coefL_ = isPannedLeft ? panUp : panDown;
    coefR_ = isPannedLeft ? panDown : panUp;
  }

protected:
  T pan_;
  T midGain_;
  T coefL_;
  T coefR_;
};

template <FloatingPoint T>
struct PanMono : public PanBase<T>, public AlgorithmBase<std::array<T, 2>> {

  PanMono(T pan = T(0)) : PanBase<T>(pan) {}

  std::array<T, 2> process(T input) {
    this->processed[0] = input * this->coefL_;
    this->processed[1] = input * this->coefR_;

    return this->processed;
  }
};

template <FloatingPoint T>
struct PanStereo : public PanBase<T>, public AlgorithmBase<std::array<T, 2>> {

  PanStereo(T pan = T(0)) : PanBase<T>(pan) {}

  std::array<T, 2> process(std::array<T, 2> input) {
    this->processed[0] = input[0] * this->coefL_;
    this->processed[1] = input[1] * this->coefR_;

    return this->processed;
  }
};

} // namespace Clover::Stereo
