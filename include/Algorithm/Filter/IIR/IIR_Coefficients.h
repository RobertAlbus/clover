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

#include "Util.h"

template <FloatingPoint T> struct IIRFilterCoefficients {
  T b0, b1, b2, a0, a1, a2;

  void normalize() {
    T a0_abs = std::abs(a0);
    if (a0 == T(1) || a0_abs == T(0) || a0_abs == INFINITY || a0_abs == NAN)
      return;

    T a0_inv = T(1) / a0;
    b0 *= a0_inv;
    b1 *= a0_inv;
    b2 *= a0_inv;
    a0 = T(1);
    a1 *= a0_inv;
    a2 *= a0_inv;
  }
};
