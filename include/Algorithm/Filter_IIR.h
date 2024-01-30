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

#include "Util/FloatingPointConcept.h"

namespace Clover::Filter {

template <FloatingPoint T> struct IIRFilterCoefficients {
  T b0, b1, b2, a0, a1, a2;
};

template <FloatingPoint T>
void normalizeCoefficients(IIRFilterCoefficients<T> &coefficients) {
  T a0_abs = std::abs(coefficients.a0);
  if (coefficients.a0 == T(1) || a0_abs == T(0) || std::isinf(a0_abs) ||
      std::isnan(a0_abs))
    return;

  T a0_inv = T(1) / coefficients.a0;

  coefficients.b0 *= a0_inv;
  coefficients.b1 *= a0_inv;
  coefficients.b2 *= a0_inv;
  coefficients.a0 = T(1);
  coefficients.a1 *= a0_inv;
  coefficients.a2 *= a0_inv;
}

template <FloatingPoint T, int __arity> class IIRFilterDF2T {
public:
  IIRFilterDF2T() : inputStates_({{0}}), outputStates_({{0}}), coeffs_({}) {}

  IIRFilterDF2T(const IIRFilterCoefficients<T> &coeffs)
      : inputStates_({{0}}), outputStates_({{0}}), coeffs_(coeffs) {}

  void updateCoefficients(IIRFilterCoefficients<T> &coeffs) {
    normalizeCoefficients<T>(coeffs);
    coeffs_ = coeffs;
  }

  std::array<T, __arity> process(const std::array<T, __arity> &input) {
    std::array<T, __arity> output;

    for (int ch = 0; ch < __arity; ++ch) {
      T yn = coeffs_.b0 * input[ch] + coeffs_.b1 * inputStates_[0][ch] +
             coeffs_.b2 * inputStates_[1][ch] -
             coeffs_.a1 * outputStates_[0][ch] -
             coeffs_.a2 * outputStates_[1][ch];

      inputStates_[1][ch] = inputStates_[0][ch];
      inputStates_[0][ch] = input[ch];
      outputStates_[1][ch] = outputStates_[0][ch];
      outputStates_[0][ch] = yn;

      output[ch] = yn;
    }

    return output;
  }

private:
  IIRFilterCoefficients<T> coeffs_;
  std::array<std::array<T, __arity>, 2> inputStates_;
  std::array<std::array<T, __arity>, 2> outputStates_;
};

/*
The following implementation is based on:
---------------------------------------------------------
Cookbook formulae for audio EQ biquad filter coefficients
by Robert Bristow-Johnson  <rbj@audioimagination.com>
*/

template <FloatingPoint T>
IIRFilterCoefficients<T> lowPass_rbj(T cutoff_hz, T Q, T samplerate_hz) {
  T omega = T(2) * std::numbers::pi_v<T> * cutoff_hz / samplerate_hz;
  T cos_omega = std::cos(omega);
  T alpha = std::sin(omega) / (T(2) * Q);

  T b0 = (T(1) - cos_omega) * T(0.5);
  T b1 = T(1) - cos_omega;
  T b2 = b0;

  T a0 = T(1) + alpha;
  T a1 = T(-2) * cos_omega;
  T a2 = T(1) - alpha;

  IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
  normalizeCoefficients(coeffs);
  return coeffs;
}

template <FloatingPoint T>
IIRFilterCoefficients<T> highPass_rbj(T cutoff_hz, T Q, T samplerate_hz) {
  T omega = T(2) * std::numbers::pi_v<T> * cutoff_hz / samplerate_hz;
  T cos_omega = std::cos(omega);
  T alpha = std::sin(omega) / (T(2) * Q);

  T b0 = (T(1) + cos_omega) * T(0.5);
  T b1 = -(T(1) + cos_omega);
  T b2 = b0;

  T a0 = T(1) + alpha;
  T a1 = T(-2) * cos_omega;
  T a2 = T(1) - alpha;

  IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
  normalizeCoefficients(coeffs);
  return coeffs;
}

// Q gain - resonant
template <FloatingPoint T>
IIRFilterCoefficients<T>
bandPassResonant_rbj(T cutoff_hz, T Q, T samplerate_hz) {
  T omega = T(2) * std::numbers::pi_v<T> * cutoff_hz / samplerate_hz;
  T cos_omega = std::cos(omega);
  T sin_omega = std::sin(omega);
  T alpha = sin_omega / (T(2) * Q);

  T b0 = sin_omega * T(0.5);
  T b1 = T(0);
  T b2 = T(-1) * b0;

  T a0 = T(1) + alpha;
  T a1 = T(-2) * cos_omega;
  T a2 = T(1) - alpha;

  IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
  normalizeCoefficients(coeffs);
  return coeffs;
}

// 0db gain - flat
template <FloatingPoint T>
IIRFilterCoefficients<T> bandPass_rbj(T cutoff_hz, T Q, T samplerate_hz) {
  T omega = T(2) * std::numbers::pi_v<T> * cutoff_hz / samplerate_hz;
  T cos_omega = std::cos(omega);
  T alpha = std::sin(omega) / (T(2) * Q);

  T b0 = alpha;
  T b1 = T(0);
  T b2 = T(-1) * alpha;

  T a0 = T(1) + alpha;
  T a1 = T(-2) * cos_omega;
  T a2 = T(1) - alpha;

  IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
  normalizeCoefficients(coeffs);
  return coeffs;
}

template <FloatingPoint T>
IIRFilterCoefficients<T> notch_rbj(T cutoff_hz, T Q, T samplerate_hz) {
  T omega = T(2) * std::numbers::pi_v<T> * cutoff_hz / samplerate_hz;
  T cos_omega = std::cos(omega);
  T alpha = std::sin(omega) / (T(2) * Q);

  T b0 = T(1);
  T b1 = T(-2) * cos_omega;
  T b2 = T(1);

  T a0 = T(1) + alpha;
  T a1 = b1;
  T a2 = T(1) - alpha;

  IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
  normalizeCoefficients(coeffs);
  return coeffs;
}

template <FloatingPoint T>
IIRFilterCoefficients<T> allPass_rbj(T cutoff_hz, T Q, T samplerate_hz) {
  T omega = T(2) * std::numbers::pi_v<T> * cutoff_hz / samplerate_hz;
  T cos_omega = std::cos(omega);
  T alpha = std::sin(omega) / (T(2) * Q);

  T b0 = T(1) - alpha;
  T b1 = T(-2) * cos_omega;
  T b2 = T(1) + alpha;

  T a0 = b2;
  T a1 = b1;
  T a2 = b0;

  IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
  normalizeCoefficients(coeffs);
  return coeffs;
}

template <FloatingPoint T>
IIRFilterCoefficients<T>
lowShelf_rbj(T cutoff_hz, T Q, T gain_db, T samplerate_hz) {
  T omega = T(2) * std::numbers::pi_v<T> * cutoff_hz / samplerate_hz;
  T cos_omega = std::cos(omega);
  T alpha = std::sin(omega) / (T(2) * Q);

  T A = std::pow(T(10), gain_db / T(40));

  T b0 =
      A * ((A + T(1)) - (A - T(1)) * cos_omega + T(2) * std::sqrt(A) * alpha);
  T b1 = T(2) * A * ((A - T(1)) - (A + T(1)) * cos_omega);
  T b2 =
      A * ((A + T(1)) - (A - T(1)) * cos_omega - T(2) * std::sqrt(A) * alpha);
  T a0 = (A + T(1)) + (A - T(1)) * cos_omega + T(2) * std::sqrt(A) * alpha;
  T a1 = -T(2) * ((A - T(1)) + (A + T(1)) * cos_omega);
  T a2 = (A + T(1)) + (A - T(1)) * cos_omega - T(2) * std::sqrt(A) * alpha;

  IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
  normalizeCoefficients(coeffs);
  return coeffs;
}

template <FloatingPoint T>
IIRFilterCoefficients<T>
highShelf_rbj(T cutoff_hz, T Q, T gain_db, T samplerate_hz) {
  T omega = T(2) * std::numbers::pi_v<T> * cutoff_hz / samplerate_hz;
  T cos_omega = std::cos(omega);
  T alpha = std::sin(omega) / (T(2) * Q);

  T A = std::pow(T(10), gain_db / T(40));

  T b0 =
      A * ((A + T(1)) + (A - T(1)) * cos_omega + T(2) * std::sqrt(A) * alpha);
  T b1 = -T(2) * A * ((A - T(1)) + (A + T(1)) * cos_omega);
  T b2 =
      A * ((A + T(1)) + (A - T(1)) * cos_omega - T(2) * std::sqrt(A) * alpha);
  T a0 = (A + T(1)) - (A - T(1)) * cos_omega + T(2) * std::sqrt(A) * alpha;
  T a1 = T(2) * ((A - T(1)) - (A + T(1)) * cos_omega);
  T a2 = (A + T(1)) - (A - T(1)) * cos_omega - T(2) * std::sqrt(A) * alpha;

  IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
  normalizeCoefficients(coeffs);
  return coeffs;
}

template <FloatingPoint T>
IIRFilterCoefficients<T>
peakingEQ_rbj(T cutoff_hz, T Q, T gain_db, T samplerate_hz) {
  T omega = T(2) * std::numbers::pi_v<T> * cutoff_hz / samplerate_hz;
  T cos_omega = std::cos(omega);
  T alpha = std::sin(omega) / (T(2) * Q);
  T A = std::pow(T(10), gain_db / T(40));

  T alphaA = alpha * A;

  T b0 = T(1) + alphaA;
  T b1 = T(-2) * cos_omega;
  T b2 = T(1) - alphaA;

  T alphaOverA = alpha / A;

  T a0 = T(1) + alphaOverA;
  T a1 = b1;
  T a2 = T(1) - alphaOverA;

  IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
  normalizeCoefficients(coeffs);
  return coeffs;
}

} // namespace Clover::Filter
