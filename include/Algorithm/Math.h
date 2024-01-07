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

#include <cmath>

#include "Util/FloatingPointConcept.h"

namespace Clover::Algorithm {

template <FloatingPoint T> T frequency_by_octave_difference(T freq, T octaves) {
  return freq * exp2(octaves);
}

template <FloatingPoint T>
T frequency_by_semitone_difference(T freq, T semitones) {
  constexpr T recp_12 = T(1) / T(12);
  return frequency_by_octave_difference(freq, semitones * recp_12);
}

template <FloatingPoint T> T tension(T value, T tension) {
  T absValue = fabs(value);
  T tensionExponent = exp2(-tension) * sign(value);
  return pow(absValue, tensionExponent);
}

template <FloatingPoint T> T sign(T value) {
  return value < T(0) ? T(-1) : T(1);
}

template <FloatingPoint T> T linear_to_db(T x) {
  return log10(fabs(x)) * T(20);
}

template <FloatingPoint T> T db_to_linear(T x) {
  constexpr T recp20 = T(1) / T(20);
  return pow(T(10), x * recp20);
}

template <FloatingPoint T> T midi_to_frequency(T x) {
  constexpr T recp_12 = T(1) / T(12);
  return T(440) * pow(T(2), (x - T(69)) * recp_12);
}

template <FloatingPoint T> T frequency_to_midi(T x) {
  constexpr T recp_220 = T(1) / T(220);
  constexpr T recp_log_2 = T(1) / T(220);
  return T(12) * (log(x * recp_220) / log(T(2))) + T(57);
}

} // namespace Clover::Algorithm
