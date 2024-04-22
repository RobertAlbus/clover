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

#include <assert.h>
#include <cmath>

#include "Clover/Util/FloatingPointConcept.h"

namespace Clover::Envelope {

template <FloatingPoint T> struct Basic {
  Basic() : Basic(0.f, 0.f, 1) {}

  Basic(float from, float to, uint numSteps) {
    set(from, to, numSteps);
    previous_ = from;
  }

  void set(float from, float to, int numSteps) {
    set(from, to, static_cast<uint>(numSteps));
  }

  void set(float from, float to, uint numSteps) {
    from_ = from;
    to_ = to;
    currentStep_ = 0;
    configureIncrement(numSteps);
  }

  void set(float to, int numSteps) { set(to, static_cast<uint>(numSteps)); }

  void set(float to, uint numSteps) {
    from_ = previous_;
    to_ = to;

    // let the previous sample be the starting the point
    currentStep_ = 1;
    configureIncrement(numSteps);
  }

  T process() {

    if (currentStep_ >= targetStep_) {
      previous_ = to_;
      return to_;
    }

    T lerpAmount = currentStep_ * reciprocal_;
    currentStep_++;

    previous_ = std::lerp(from_, to_, lerpAmount);

    return previous_;
  }

protected:
  T from_;
  T to_;
  T previous_;
  T reciprocal_;
  uint currentStep_;
  uint targetStep_;

  void configureIncrement(uint numSteps) {
    numSteps = numSteps < 2 ? 2 : numSteps;

    reciprocal_ = 1 / static_cast<T>(numSteps - 1);
    targetStep_ = numSteps;
  }
};

} // namespace Clover::Envelope
