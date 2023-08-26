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
#include <array>
#include <cmath>
#include <optional>
#include <vector>

#include "IIRCoefficientStrategy.h"
#include "IIR_Coefficients.h"
#include "Util.h"

namespace Clover::Filter {

template <FloatingPoint T>
struct ChebyshevType1CoefficientStrategy : public IIRCoefficientStrategy<T> {

  T stopBandAttenuationDb = T(60);

  IIRFilterCoefficients<T> lowPass(T cutoff_hz, T Q, T samplerate_hz) {
    T ripple = std::sqrt(std::pow(T(10), T(0.1) * Q) - T(1));
    T epsilon = std::sqrt(std::pow(T(10), T(0.1) * ripple) - T(1));
    T A = std::pow(T(10), -0.05 * ripple);
    T omega_c = T(2) * M_PI * cutoff_hz / samplerate_hz;
    T alpha = T(1) / std::sinh(std::asinh(T(1) / epsilon) / T(2));

    T b0 = A * (T(1) - alpha);
    T b1 = T(2) * A * (alpha - std::cos(omega_c));
    T b2 = A * (T(1) - alpha);
    T a0 = T(1) + alpha;
    T a1 = T(-2) * std::cos(omega_c);
    T a2 = T(1) - alpha;

    IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
    coeffs.normalize();

    return coeffs;
  }

  IIRFilterCoefficients<T> highPass(T cutoff_hz, T Q, T samplerate_hz) {
    return highPass(cutoff_hz, Q, samplerate_hz, std::nullopt);
  }

  IIRFilterCoefficients<T>
  highPass(T cutoff_hz, T Q, T samplerate_hz,
           std::optional<IIRFilterCoefficients<T>> lp_coefficients) {

    if (!lp_coefficients.has_value()) {
      lp_coefficients = lowPass(cutoff_hz, Q, samplerate_hz);
    }

    T k = std::tan(M_PI * cutoff_hz / samplerate_hz) /
          std::tan(M_PI * (samplerate_hz - cutoff_hz) / samplerate_hz);
    T denominator = k * k + T(2) * k + T(1);

    T b0 = lp_coefficients->b0 / denominator;
    T b1 = T(-2) * lp_coefficients->b1 / denominator;
    T b2 = lp_coefficients->b2 / denominator;
    T a0 = lp_coefficients->a0;
    T a1 = (T(2) * (k * k - T(1))) / denominator;
    T a2 = (k * k - T(2) * k + T(1)) / denominator;

    IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
    coeffs.normalize();

    return coeffs;
  }

  IIRFilterCoefficients<T> bandPass(T cutoff_hz, T Q, T samplerate_hz) {
    IIRFilterCoefficients<T> lp_coefficients =
        lowPass(cutoff_hz, Q, samplerate_hz);
    IIRFilterCoefficients<T> hp_coefficients =
        highPass(cutoff_hz, Q, samplerate_hz, lp_coefficients);

    T b0 = lp_coefficients.b1 * hp_coefficients.b1;
    T b1 = lp_coefficients.b1 * hp_coefficients.b0 +
           lp_coefficients.b0 * hp_coefficients.b1;
    T b2 = lp_coefficients.b0 * hp_coefficients.b0;
    T a0 = T(1);
    T a1 = -(lp_coefficients.a1 + hp_coefficients.a1);
    T a2 = lp_coefficients.a2 + hp_coefficients.a2;

    IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};

    return coeffs;
  }

  IIRFilterCoefficients<T> notch(T cutoff_hz, T Q, T samplerate_hz) {
    IIRFilterCoefficients<T> lp_coefficients =
        lowPass(cutoff_hz / T(2), Q, samplerate_hz);
    IIRFilterCoefficients<T> hp_coefficients =
        highPass(cutoff_hz / T(2), Q, samplerate_hz, lp_coefficients);

    T b0 = lp_coefficients.b0 + hp_coefficients.b0;
    T b1 = lp_coefficients.b1 + hp_coefficients.b1;
    T b2 = lp_coefficients.b2 + hp_coefficients.b2;
    T a0 = T(1);
    T a1 = (lp_coefficients.a1 + hp_coefficients.a1) / T(2);
    T a2 = lp_coefficients.a2 * hp_coefficients.a2;

    IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
    coeffs.normalize();

    return coeffs;
  }

  IIRFilterCoefficients<T> lowShelf(T cutoff_hz, T Q, T gain_db,
                                    T samplerate_hz) {
    IIRFilterCoefficients<T> lp_coefficients =
        lowPass(cutoff_hz / T(2), Q, samplerate_hz);
    IIRFilterCoefficients<T> hp_coefficients =
        highPass(cutoff_hz / T(2), Q, samplerate_hz, lp_coefficients);

    T linear_gain = std::pow(T(10), gain_db / 20);
    T b0 = lp_coefficients.b0 * linear_gain + hp_coefficients.b0;
    T b1 = lp_coefficients.b1 * linear_gain + hp_coefficients.b1;
    T b2 = lp_coefficients.b2 * linear_gain + hp_coefficients.b2;
    T a0 = T(1);
    T a1 = (lp_coefficients.a1 + hp_coefficients.a1) / T(2);
    T a2 = lp_coefficients.a2 * hp_coefficients.a2;

    IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};

    return coeffs;
  }

  IIRFilterCoefficients<T> highShelf(T cutoff_hz, T Q, T gain_db,
                                     T samplerate_hz) {
    IIRFilterCoefficients<T> lp_coefficients =
        lowPass(cutoff_hz / T(2), Q, samplerate_hz);
    IIRFilterCoefficients<T> hp_coefficients =
        highPass(cutoff_hz / T(2), Q, samplerate_hz, lp_coefficients);

    T linear_gain = std::pow(T(10), gain_db / T(20));
    T b0 = hp_coefficients.b0 * linear_gain - lp_coefficients.b0;
    T b1 = hp_coefficients.b1 * linear_gain - lp_coefficients.b1;
    T b2 = hp_coefficients.b2 * linear_gain - lp_coefficients.b2;
    T a0 = T(1);
    T a1 = (lp_coefficients.a1 + hp_coefficients.a1) / T(2);
    T a2 = lp_coefficients.a2 * hp_coefficients.a2;

    IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};

    return coeffs;
  }

  IIRFilterCoefficients<T> peakingEQ(T cutoff_hz, T Q, T gain_db,
                                     T samplerate_hz) {
    IIRFilterCoefficients<T> lp_coefficients =
        lowPass(cutoff_hz, Q, samplerate_hz);

    T K = std::tan(M_PI * cutoff_hz / samplerate_hz);
    T V = std::pow(T(10), gain_db / 20);

    T b0 = (T(1) + K / Q + V * K * K) * lp_coefficients.b0 /
           (T(1) + K / Q / lp_coefficients.a1 + K * K / lp_coefficients.a2);
    T b1 = (T(2) * (V * K * K - T(1)) * lp_coefficients.b0) /
           (T(1) + K / Q / lp_coefficients.a1 + K * K / lp_coefficients.a2);
    T b2 = ((T(1) - K / Q + V * K * K) * lp_coefficients.b0) /
           (T(1) + K / Q / lp_coefficients.a1 + K * K / lp_coefficients.a2);
    T a0 = T(1);
    T a1 = (T(2) * (K * K / lp_coefficients.a2 - T(1))) /
           (T(1) + K / Q / lp_coefficients.a1 + K * K / lp_coefficients.a2);
    T a2 = (K / Q / lp_coefficients.a1 + K * K / lp_coefficients.a2 - T(1)) /
           (T(1) + K / Q / lp_coefficients.a1 + K * K / lp_coefficients.a2);

    IIRFilterCoefficients<T> coefficients{b0, b1, b2, a0, a1, a2};

    return coefficients;
  }
};

} // namespace Clover::Filter
