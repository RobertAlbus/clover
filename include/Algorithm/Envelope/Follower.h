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
#include <numbers>

#include "Algorithm/Filter/IIR/IIRFilterDF2T.h"
#include "Algorithm/Filter/IIR/RbjBiquadCoefficients.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::Envelope {

template <FloatingPoint T, int __arity> struct Follower {

Follower(T fs, T smoothing_ms = 100) : m_fs(fs) {

}

  void smooth_ms(T ms) {
    T t = ms * 0.001;
    T f0 = T(1) / (T(2) * std::numbers::pi_v<T> * t);
    m_filter.updateCoefficients(
      m_coefficients.lowPass(f0, 0.707, m_fs));
  }

  void smooth_samples(T N) {
    T t = N / m_fs;
    T f0 = T(1) / (T(2) * std::numbers::pi_v<T> * t);
    m_filter.updateCoefficients(
      m_coefficients.lowPass(f0, 0.707, m_fs));
  }

  void smooth_freq(T f0) {
    m_filter.updateCoefficients(
m_coefficients.lowPass(f0, 0.707, m_fs));
  }

  std::array<T, __arity> process(std::array<T, __arity> input) {
    fullWaveRectify(input);
    return m_filter.process(input);
  }

private:
  Filter::IIRFilterDF2T<T, __arity> m_filter;
  Filter::RbjBiquadCoefficientStrategy<T> m_coefficients;
  T m_fs;

  void fullWaveRectify(std::array<T, __arity>& input) {
    for (int i = 0; i < __arity; ++i) {
      input[i] = std::fabs(input[i]);
    }
  }
};

} // namespace Clover::Envelope
