#pragma once

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
struct ChebyshevType2CoefficientStrategy : public IIRCoefficientStrategy<T> {

  T stopBandAttenuationDb = T(60);

  IIRFilterCoefficients<T> lowPass(T cutoff_hz, T Q, T samplerate_hz) {
    IIRFilterCoefficients<T> coeffs;
    T K = std::tan(M_PI * cutoff_hz / samplerate_hz);
    T K2 = K * K;
    T V0 = std::sqrt(1 + Q * Q * K2) + Q * K;
    T V1 = 2 * std::sqrt(1 + Q * Q * K2);
    T V2 = std::sqrt(1 + Q * Q * K2) - Q * K;
    T den = 1 / (1 + V0 + V1 + V2);

    coeffs.b0 = den;
    coeffs.b1 = 2 * den;
    coeffs.b2 = den;
    coeffs.a0 = 1;
    coeffs.a1 = (-2 + 2 * V0 - V1) * den;
    coeffs.a2 = (1 - V0 + V1 - V2) * den;

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
    T denominator = k * k + 2 * k + 1;

    T b0 = lp_coefficients->b0 / denominator;
    T b1 = -2 * lp_coefficients->b1 / denominator;
    T b2 = lp_coefficients->b2 / denominator;
    T a0 = lp_coefficients->a0;
    T a1 = (2 * (k * k - 1)) / denominator;
    T a2 = (k * k - 2 * k + 1) / denominator;

    IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
    coeffs.normalize();

    return coeffs;
  }

  IIRFilterCoefficients<T> bandPass(T cutoff_hz, T Q, T samplerate_hz) {
    IIRFilterCoefficients<T> lp_coefficients =
        lowPass(cutoff_hz, Q, samplerate_hz);
    IIRFilterCoefficients<T> hp_coefficients =
        highPass(cutoff_hz, Q, samplerate_hz, lp_coefficients);

    T b0 = lp_coefficients.b0 * hp_coefficients.b0;
    T b1 = lp_coefficients.b0 * hp_coefficients.b1 +
           lp_coefficients.b1 * hp_coefficients.b0;
    T b2 = lp_coefficients.b0 * hp_coefficients.b2 +
           lp_coefficients.b1 * hp_coefficients.b1 +
           lp_coefficients.b2 * hp_coefficients.b0;
    T a0 = lp_coefficients.a0 * hp_coefficients.a0;
    T a1 = lp_coefficients.a1 + hp_coefficients.a1;
    T a2 = lp_coefficients.a1 * hp_coefficients.a1 + lp_coefficients.a2 +
           hp_coefficients.a2;

    IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
    coeffs.normalize();

    return coeffs;
  }

  IIRFilterCoefficients<T> notch(T cutoff_hz, T Q, T samplerate_hz) {
    IIRFilterCoefficients<T> lp_coefficients =
        lowPass(cutoff_hz / 2, Q, samplerate_hz);

    IIRFilterCoefficients<T> hp_coefficients =
        highPass(cutoff_hz * 2, Q, samplerate_hz, lp_coefficients);

    T b0 = lp_coefficients.b0 + hp_coefficients.b0;
    T b1 = lp_coefficients.b1 + hp_coefficients.b1;
    T b2 = lp_coefficients.b2 + hp_coefficients.b2;
    T a0 = T(1);
    T a1 = (lp_coefficients.a1 + hp_coefficients.a1) / 2;
    T a2 = lp_coefficients.a2 * hp_coefficients.a2;

    IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
    coeffs.normalize();

    return coeffs;
  }

  IIRFilterCoefficients<T> lowShelf(T cutoff_hz, T Q, T gain_db,
                                    T samplerate_hz) {
    IIRFilterCoefficients<T> lp_coefficients =
        lowPass(cutoff_hz, Q, samplerate_hz);
    IIRFilterCoefficients<T> hp_coefficients =
        highPass(cutoff_hz, Q, samplerate_hz, lp_coefficients);

    T linear_gain = std::pow(10, gain_db / 20);
    T b0 = lp_coefficients.b0 * linear_gain + hp_coefficients.b0;
    T b1 = lp_coefficients.b1 * linear_gain + hp_coefficients.b1;
    T b2 = lp_coefficients.b2 * linear_gain + hp_coefficients.b2;
    T a0 = T(1);
    T a1 = (lp_coefficients.a1 + hp_coefficients.a1) / 2;
    T a2 = lp_coefficients.a2 * hp_coefficients.a2;

    IIRFilterCoefficients<T> coefficients{b0, b1, b2, a0, a1, a2};

    return coefficients;
  }

  IIRFilterCoefficients<T> highShelf(T cutoff_hz, T Q, T gain_db,
                                     T samplerate_hz) {
    IIRFilterCoefficients<T> lp_coefficients =
        lowPass(cutoff_hz, Q, samplerate_hz);
    IIRFilterCoefficients<T> hp_coefficients =
        highPass(cutoff_hz, Q, samplerate_hz, lp_coefficients);

    T linear_gain = std::pow(10, gain_db / 20);
    T b0 = lp_coefficients.b0 - hp_coefficients.b0 * linear_gain;
    T b1 = lp_coefficients.b1 - hp_coefficients.b1 * linear_gain;
    T b2 = lp_coefficients.b2 - hp_coefficients.b2 * linear_gain;
    T a0 = T(1);
    T a1 = (lp_coefficients.a1 + hp_coefficients.a1) / 2;
    T a2 = lp_coefficients.a2 * hp_coefficients.a2;

    IIRFilterCoefficients<T> coeffs{b0, b1, b2, a0, a1, a2};
    coeffs.normalize();

    return coeffs;
  }

  IIRFilterCoefficients<T> peakingEQ(T cutoff_hz, T Q, T gain_db,
                                     T samplerate_hz) {
    IIRFilterCoefficients<T> bp_coefs = bandPass(cutoff_hz, Q, samplerate_hz);

    T V = std::pow(10, gain_db / 20);

    T b0 = (1 + V * bp_coefs.b0) / (1 + bp_coefs.a1 + bp_coefs.a2);
    T b1 = (bp_coefs.b1) / (1 + bp_coefs.a1 + bp_coefs.a2);
    T b2 = (1 + V * bp_coefs.b2) / (1 + bp_coefs.a1 + bp_coefs.a2);
    T a0 = T(1);
    T a1 = (bp_coefs.a1) / (1 + bp_coefs.a1 + bp_coefs.a2);
    T a2 = (bp_coefs.a2) / (1 + bp_coefs.a1 + bp_coefs.a2);

    IIRFilterCoefficients<T> coefficients{b0, b1, b2, a0, a1, a2};

    return coefficients;
  }
};

} // namespace Clover::Filter