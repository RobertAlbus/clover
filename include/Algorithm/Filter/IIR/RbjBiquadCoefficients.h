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
#include <numbers>

#include "IIR_Coefficients.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::Filter {

/*
The following implementation is based on:
---------------------------------------------------------
Cookbook formulae for audio EQ biquad filter coefficients
by Robert Bristow-Johnson  <rbj@audioimagination.com>
*/

template <FloatingPoint T> struct RbjBiquadCoefficientStrategy {
  IIRFilterCoefficients<T> lowPass(T cutoff_hz, T Q, T samplerate_hz) {
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
    coeffs.normalize();
    return coeffs;
  }

  IIRFilterCoefficients<T> highPass(T cutoff_hz, T Q, T samplerate_hz) {
    T omega = T(2) * std::numbers::pi_v<T> * cutoff_hz / samplerate_hz;
    T cos_omega = std::cos(omega);
    T alpha = std::sin(omega) / (T(2) * Q);

    T b0 = (T(1) + cos_omega) * T(0.5);
    T b1 = - (T(1) + cos_omega);
    T b2 = b0;

    T a0 = T(1) + alpha;
    T a1 = T(-2) * cos_omega;
    T a2 = T(1) - alpha;

    IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
    coeffs.normalize();
    return coeffs;
  }

  // Q gain - resonant
  IIRFilterCoefficients<T> bandPassResonant(T cutoff_hz, T Q, T samplerate_hz) {
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
    coeffs.normalize();
    return coeffs;
  }

  // 0db gain - flat
  IIRFilterCoefficients<T> bandPass(T cutoff_hz, T Q, T samplerate_hz) {
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
    coeffs.normalize();
    return coeffs;
  }

  IIRFilterCoefficients<T> notch(T cutoff_hz, T Q, T samplerate_hz) {
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
    coeffs.normalize();
    return coeffs;
  }

  IIRFilterCoefficients<T> allPass(T cutoff_hz, T Q, T samplerate_hz) {
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
    coeffs.normalize();
    return coeffs;
  }

  IIRFilterCoefficients<T>
  lowShelf(T cutoff_hz, T Q, T gain_db, T samplerate_hz) {
    T omega = T(2) * std::numbers::pi_v<T> * cutoff_hz / samplerate_hz;
    T cos_omega = std::cos(omega);
    T alpha = std::sin(omega) / (T(2) * Q);

    T A = std::pow(T(10), gain_db / T(40));
    // T twoSqrtAAlpha = T(2) * std::sqrt(A) * alpha;
    // T sqrtAAlpha = std::sqrt(A) * alpha;
    // T b0_b2_part1 = (A + T(1)) - (A - T(1)) * cos_omega;

    ////////////////////
    T b0 =
        A * ((A + T(1)) - (A - T(1)) * cos_omega + T(2) * std::sqrt(A) * alpha);
    T b1 = T(2) * A * ((A - T(1)) - (A + T(1)) * cos_omega);
    T b2 =
        A * ((A + T(1)) - (A - T(1)) * cos_omega - T(2) * std::sqrt(A) * alpha);
    T a0 = (A + T(1)) + (A - T(1)) * cos_omega + T(2) * std::sqrt(A) * alpha;
    T a1 = -T(2) * ((A - T(1)) + (A + T(1)) * cos_omega);
    T a2 = (A + T(1)) + (A - T(1)) * cos_omega - T(2) * std::sqrt(A) * alpha;
    ////////////////////

    // T b0 = A * (b0_b2_part1 + twoSqrtAAlpha);
    // T b1 = T(2) * A * ((A - T(1)) - (A + T(1)) * cos_omega);
    // T b2 = A * (b0_b2_part1 - twoSqrtAAlpha);

    // T a0_a2_part1 = (A + T(1)) + (A - T(1)) * cos_omega;

    // T a0 = a0_a2_part1 + twoSqrtAAlpha;
    // T a1 = T(-2) * A * ((A - T(1)) + (A + T(1)) * cos_omega);
    // T a2 = a0_a2_part1 - twoSqrtAAlpha;

    IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
    coeffs.normalize();
    return coeffs;
  }

  IIRFilterCoefficients<T>
  highShelf(T cutoff_hz, T Q, T gain_db, T samplerate_hz) {
    T omega = T(2) * std::numbers::pi_v<T> * cutoff_hz / samplerate_hz;
    T cos_omega = std::cos(omega);
    T alpha = std::sin(omega) / (T(2) * Q);

    T A = std::pow(T(10), gain_db / T(40));
    // T twoSqrtAAlpha = T(2) * std::sqrt(A) * alpha;
    // T sqrtAAlpha = std::sqrt(A) * alpha;
    // T b0_b2_part1 = (A + T(1)) + (A - T(1)) * cos_omega;

    ////////////////
    T b0 =
        A * ((A + T(1)) + (A - T(1)) * cos_omega + T(2) * std::sqrt(A) * alpha);
    T b1 = -T(2) * A * ((A - T(1)) + (A + T(1)) * cos_omega);
    T b2 =
        A * ((A + T(1)) + (A - T(1)) * cos_omega - T(2) * std::sqrt(A) * alpha);
    T a0 = (A + T(1)) - (A - T(1)) * cos_omega + T(2) * std::sqrt(A) * alpha;
    T a1 = T(2) * ((A - T(1)) - (A + T(1)) * cos_omega);
    T a2 = (A + T(1)) - (A - T(1)) * cos_omega - T(2) * std::sqrt(A) * alpha;
    ////////////////

    // T b0 = A * (b0_b2_part1 + twoSqrtAAlpha);
    // T b1 = T(-2) * A * ((A - T(1)) + (A + T(1)) * cos_omega);
    // T b2 = A * (b0_b2_part1 - twoSqrtAAlpha);

    // T a0_a2_part1 = (A + T(1)) - (A - T(1))  * cos_omega;

    // T a0 = a0_a2_part1 + twoSqrtAAlpha;
    // T a1 = T(2) * A * ((A - T(1)) - (A + T(1)) * cos_omega);
    // T a2 = a0_a2_part1 - twoSqrtAAlpha;

    IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
    coeffs.normalize();
    return coeffs;
  }

  IIRFilterCoefficients<T>
  peakingEQ(T cutoff_hz, T Q, T gain_db, T samplerate_hz) {
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
    coeffs.normalize();
    return coeffs;
  }
};

} // namespace Clover::Filter
