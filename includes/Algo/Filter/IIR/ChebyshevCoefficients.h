#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

#include "IIR_Coefficients.h"
#include "Util.h"

template <FloatingPoint T>
IIRFilterCoefficients<T>
chebyLowPass(T cutoff_hz, T Q, T stopband_attenuation_db, T samplerate_hz) {
  // Type 2 Chebyshev
  T normalized_frequency = 2 * cutoff_hz / samplerate_hz;
  T epsilon = std::sqrt(std::pow(10, -0.1 * stopband_attenuation_db) - 1);
  T v0 = asinh(1 / epsilon) / 2;
  T sinh_v0 = std::sinh(v0);
  T cosh_v0 = std::cosh(v0);

  T phi = M_PI / 2;
  T beta = sinh_v0 * std::sin(phi);
  T gamma = cosh_v0 * std::cos(phi);
  T sigma = beta + gamma;

  // Convert poles to the digital domain
  T k = std::tan(M_PI * normalized_frequency / 2);
  T k_sq = k * k;
  T real = sigma * k + beta * k_sq;
  T imag = gamma * k_sq - beta * k;
  T real_sq = real * real;
  T imag_sq = imag * imag;
  T denominator = real_sq + imag_sq;

  // Compute the coefficients
  T b0_lp = (beta * k_sq) / denominator;
  T b1_lp = 2 * b0_lp;
  T b2_lp = b0_lp;
  T a1_lp = 2 * (k_sq - real) / denominator;
  T a2_lp = (real_sq + imag_sq - 2 * real * k_sq) / denominator;

  T gain = 1 / (1 + a1_lp + a2_lp);

  return IIRFilterCoefficients<T>{gain, b0_lp, b1_lp, b2_lp, a1_lp, a2_lp};
}

template <FloatingPoint T>
IIRFilterCoefficients<T>
chebyHighPass(T cutoff_hz, T Q, T stopband_attenuation_db, T samplerate_hz) {
  // Compute low-pass coefficients for the Chebyshev Type II filter
  IIRFilterCoefficients<T> lp_coefficients =
      chebyType2LowPass(cutoff_hz, Q, stopband_attenuation_db, samplerate_hz);

  // Perform the low-pass to high-pass transformation
  T k = std::tan(M_PI * cutoff_hz / samplerate_hz) /
        std::tan(M_PI * (samplerate_hz - cutoff_hz) / samplerate_hz);
  T denominator = k * k + 2 * k + 1;

  T b0_hp = lp_coefficients.b0 / denominator;
  T b1_hp = -2 * lp_coefficients.b1 / denominator;
  T b2_hp = lp_coefficients.b2 / denominator;
  T a1_hp = (2 * (k * k - 1)) / denominator;
  T a2_hp = (k * k - 2 * k + 1) / denominator;

  return IIRFilterCoefficients<T>{
      lp_coefficients.gain, b0_hp, b1_hp, b2_hp, a1_hp, a2_hp};
}