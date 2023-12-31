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

#include "IIRCoefficientStrategy.h"
#include "IIR_Coefficients.h"
#include "Util.h"

namespace Clover::Filter {

template <FloatingPoint T>
struct ButterworthCoefficientStrategy : public IIRCoefficientStrategy<T> {
  IIRFilterCoefficients<T> lowPass(T cutoff_hz, T Q, T samplerate_hz) {
    T omega_c = std::tan(M_PI * cutoff_hz / samplerate_hz);
    T alpha = omega_c / Q;
    T denominator = T(1) + alpha;

    T b0 = omega_c / (T(2) * denominator);
    T b1 = omega_c / denominator;
    T b2 = b0;
    T a0 = T(1);
    T a1 = (T(2) * (omega_c * omega_c - T(1))) / denominator;
    T a2 = (T(1) - alpha) / denominator;

    return IIRFilterCoefficients<T>{b0, b1, b2, a0, a1, a2};
  }

  IIRFilterCoefficients<T> highPass(T cutoff_hz, T Q, T samplerate_hz) {
    T omega_c = std::tan(M_PI * cutoff_hz / samplerate_hz);
    T alpha = omega_c / Q;
    T denominator = T(1) + alpha;

    T b0 = T(1) / (T(2) * denominator);
    T b1 = -T(2) * b0;
    T b2 = b0;
    T a0 = T(1);
    T a1 = (T(2) * (omega_c * omega_c - T(1))) / denominator;
    T a2 = (T(1) - alpha) / denominator;

    return IIRFilterCoefficients<T>{b0, b1, b2, a0, a1, a2};
  }

  IIRFilterCoefficients<T> bandPass(T cutoff_hz, T Q, T samplerate_hz) {
    T omega_c = std::tan(M_PI * cutoff_hz / samplerate_hz);
    T alpha = omega_c / Q;
    T denominator = T(1) + alpha;

    T b0 = alpha / denominator;
    T b1 = T(0);
    T b2 = -b0;
    T a0 = T(1);
    T a1 = (T(2) * (omega_c * omega_c - T(1))) / denominator;
    T a2 = (T(1) - alpha) / denominator;

    return IIRFilterCoefficients<T>{b0, b1, b2, a0, a1, a2};
  }

  IIRFilterCoefficients<T> notch(T cutoff_hz, T Q, T samplerate_hz) {
    T omega_c = std::tan(M_PI * cutoff_hz / samplerate_hz);
    T alpha = omega_c / Q;
    T denominator = T(1) + alpha;

    T b0 = T(1);
    T b1 = (T(2) * (omega_c * omega_c - T(1))) / denominator;
    T b2 = (T(1) - alpha) / denominator;
    T a0 = T(1);
    T a1 = b1;
    T a2 = b2;

    return IIRFilterCoefficients<T>{b0, b1, b2, a0, a1, a2};
  }

  IIRFilterCoefficients<T>
  lowShelf(T cutoff_hz, T Q, T gain_db, T samplerate_hz) {
    T A = std::pow(T(10), gain_db / T(40));
    T omega_c = std::tan(M_PI * cutoff_hz / samplerate_hz);
    T beta = std::sqrt(A) / Q;
    T denominator = T(1) + omega_c / beta + omega_c * omega_c;

    T b0 = A * (T(1) + omega_c / beta + omega_c * omega_c) / denominator;
    T b1 = T(2) * A * (omega_c * omega_c - T(1)) / denominator;
    T b2 = A * (T(1) - omega_c / beta + omega_c * omega_c) / denominator;
    T a0 = T(1);
    T a1 = T(2) * (omega_c * omega_c - T(1)) / denominator;
    T a2 = (T(1) - omega_c / beta + omega_c * omega_c) / denominator;

    return IIRFilterCoefficients<T>{b0, b1, b2, a0, a1, a2};
  }

  IIRFilterCoefficients<T>
  highShelf(T cutoff_hz, T Q, T gain_db, T samplerate_hz) {
    T A = std::pow(T(10), gain_db / T(40));
    T omega_c = std::tan(M_PI * cutoff_hz / samplerate_hz);
    T beta = std::sqrt(A) / Q;
    T denominator = T(1) + omega_c / beta + omega_c * omega_c;

    T b0 = A * (T(1) - omega_c / beta + omega_c * omega_c) / denominator;
    T b1 = T(2) * A * (T(1) - omega_c * omega_c) / denominator;
    T b2 = A * (T(1) + omega_c / beta + omega_c * omega_c) / denominator;
    T a0 = T(1);
    T a1 = T(2) * (omega_c * omega_c - T(1)) / denominator;
    T a2 = (T(1) - omega_c / beta + omega_c * omega_c) / denominator;

    return IIRFilterCoefficients<T>{b0, b1, b2, a0, a1, a2};
  }

  IIRFilterCoefficients<T>
  peakingEQ(T cutoff_hz, T Q, T gain_db, T samplerate_hz) {
    T A = std::pow(T(10), gain_db / T(40));
    T omega_c = std::tan(M_PI * cutoff_hz / samplerate_hz);
    T sin_omega_c = std::sin(omega_c);
    T alpha = sin_omega_c / (T(2) * Q);

    T denominator = T(1) + alpha / A;

    T b0 = (T(1) + alpha * A) / denominator;
    T b1 = T(-2) * std::cos(omega_c) / denominator;
    T b2 = (T(1) - alpha * A) / denominator;
    T a0 = T(1);
    T a1 = T(2) * std::cos(omega_c) / denominator;
    T a2 = (T(1) - alpha / A) / denominator;

    return IIRFilterCoefficients<T>{b0, b1, b2, a0, a1, a2};
  }
};

} // namespace Clover::Filter
