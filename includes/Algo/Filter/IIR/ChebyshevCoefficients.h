#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <optional>
#include <vector>

#include "IIR_Coefficients.h"
#include "Util.h"

template <FloatingPoint T>
IIRFilterCoefficients<T>
ChebyshevLowPass(T cutoff_hz, T Q, T stopband_attenuation_db, T samplerate_hz) {
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

  T b0_lp = (beta * k_sq) / denominator;
  T b1_lp = 2 * b0_lp;
  T b2_lp = b0_lp;
  T a1_lp = 2 * (k_sq - real) / denominator;
  T a2_lp = (real_sq + imag_sq - 2 * real * k_sq) / denominator;

  T gain = std::pow(10, -0.1 * stopband_attenuation_db);

  return IIRFilterCoefficients<T>{gain, b0_lp, b1_lp, b2_lp, a1_lp, a2_lp};
}

template <FloatingPoint T>
IIRFilterCoefficients<T> ChebyshevType2HighPass(
    T cutoff_hz, T Q, T stopband_attenuation_db, T samplerate_hz,
    std::optional<IIRFilterCoefficients<T>> lp_coefficients = std::nullopt) {

  if (!lp_coefficients.has_value()) {
    lp_coefficients =
        ChebyshevLowPass(cutoff_hz, Q, stopband_attenuation_db, samplerate_hz);
  }

  T k = std::tan(M_PI * cutoff_hz / samplerate_hz) /
        std::tan(M_PI * (samplerate_hz - cutoff_hz) / samplerate_hz);
  T denominator = k * k + 2 * k + 1;

  T b0_hp = lp_coefficients->b0 / denominator;
  T b1_hp = -2 * lp_coefficients->b1 / denominator;
  T b2_hp = lp_coefficients->b2 / denominator;
  T a1_hp = (2 * (k * k - 1)) / denominator;
  T a2_hp = (k * k - 2 * k + 1) / denominator;

  return IIRFilterCoefficients<T>{
      lp_coefficients->gain, b0_hp, b1_hp, b2_hp, a1_hp, a2_hp};
}

template <FloatingPoint T>
IIRFilterCoefficients<T>
ChebyshevType2BandPass(T low_cutoff_hz, T high_cutoff_hz, T Q,
                       T stopband_attenuation_db, T samplerate_hz) {
  IIRFilterCoefficients<T> lp_coefficients = ChebyshevLowPass(
      high_cutoff_hz, Q, stopband_attenuation_db, samplerate_hz);
  IIRFilterCoefficients<T> hp_coefficients =
      ChebyshevHighPass(low_cutoff_hz, Q, stopband_attenuation_db,
                        samplerate_hz, lp_coefficients);

  T b0_bp = lp_coefficients.b0 * hp_coefficients.b0;
  T b1_bp = lp_coefficients.b0 * hp_coefficients.b1 +
            lp_coefficients.b1 * hp_coefficients.b0;
  T b2_bp = lp_coefficients.b0 * hp_coefficients.b2 +
            lp_coefficients.b1 * hp_coefficients.b1 +
            lp_coefficients.b2 * hp_coefficients.b0;
  T a1_bp = lp_coefficients.a1 + hp_coefficients.a1;
  T a2_bp = lp_coefficients.a1 * hp_coefficients.a1 + lp_coefficients.a2 +
            hp_coefficients.a2;

  T gain = lp_coefficients.gain * hp_coefficients.gain;

  return IIRFilterCoefficients<T>{gain, b0_bp, b1_bp, b2_bp, a1_bp, a2_bp};
}

template <FloatingPoint T>
IIRFilterCoefficients<T> ChebyshevType2Notch(T center_frequency_hz, T Q,
                                             T stopband_attenuation_db,
                                             T samplerate_hz) {
  IIRFilterCoefficients<T> lp_coefficients = ChebyshevLowPass(
      center_frequency_hz / 2, Q, stopband_attenuation_db, samplerate_hz);

  IIRFilterCoefficients<T> hp_coefficients = ChebyshevType2HighPass(
      center_frequency_hz * 2, Q, stopband_attenuation_db, samplerate_hz,
      lp_coefficients);

  T b0_notch = lp_coefficients.b0 + hp_coefficients.b0;
  T b1_notch = lp_coefficients.b1 + hp_coefficients.b1;
  T b2_notch = lp_coefficients.b2 + hp_coefficients.b2;
  T a1_notch = (lp_coefficients.a1 + hp_coefficients.a1) / 2;
  T a2_notch = lp_coefficients.a2 * hp_coefficients.a2;

  return IIRFilterCoefficients<T>{1.0, b0_notch, b1_notch, b2_notch, a1_notch, a2_notch};
}

template <FloatingPoint T>
IIRFilterCoefficients<T> ChebyshevType2HighShelf(T center_frequency_hz, T Q,
                                                 T stopband_attenuation_db,
                                                 T samplerate_hz) {
  IIRFilterCoefficients<T> lp_coefficients = ChebyshevLowPass(
      center_frequency_hz, Q, stopband_attenuation_db, samplerate_hz);
  IIRFilterCoefficients<T> hp_coefficients =
      ChebyshevType2HighPass(center_frequency_hz, Q, stopband_attenuation_db,
                             samplerate_hz, lp_coefficients);

  T b0_hs = lp_coefficients.b0 - hp_coefficients.b0;
  T b1_hs = lp_coefficients.b1 - hp_coefficients.b1;
  T b2_hs = lp_coefficients.b2 - hp_coefficients.b2;
  T a1_hs = (lp_coefficients.a1 + hp_coefficients.a1) / 2;
  T a2_hs = lp_coefficients.a2 * hp_coefficients.a2;

  return IIRFilterCoefficients<T>{1.0, b0_hs, b1_hs, b2_hs, a1_hs, a2_hs};
}

template <FloatingPoint T>
IIRFilterCoefficients<T> ChebyshevType2LowShelf(T center_frequency_hz, T Q,
                                                T stopband_attenuation_db,
                                                T samplerate_hz) {
  IIRFilterCoefficients<T> lp_coefficients = ChebyshevLowPass(
      center_frequency_hz, Q, stopband_attenuation_db, samplerate_hz);

  IIRFilterCoefficients<T> hp_coefficients =
      ChebyshevType2HighPass(center_frequency_hz, Q, stopband_attenuation_db,
                             samplerate_hz, lp_coefficients);

  T b0_ls = lp_coefficients.b0 + hp_coefficients.b0;
  T b1_ls = lp_coefficients.b1 + hp_coefficients.b1;
  T b2_ls = lp_coefficients.b2 + hp_coefficients.b2;
  T a1_ls = (lp_coefficients.a1 + hp_coefficients.a1) / 2;
  T a2_ls = lp_coefficients.a2 * hp_coefficients.a2;

  return IIRFilterCoefficients<T>{1.0, b0_ls, b1_ls, b2_ls, a1_ls, a2_ls};
}

template <FloatingPoint T>
IIRFilterCoefficients<T>
ChebyshevType2PeakingEQ(T center_frequency_hz, T Q, T gain_db,
                        T stopband_attenuation_db, T samplerate_hz) {
  IIRFilterCoefficients<T> bp_coefs = ChebyshevType2BandPass(
      center_frequency_hz, Q, stopband_attenuation_db, samplerate_hz);

  T K = std::tan(M_PI * center_frequency_hz / samplerate_hz);
  T V = std::pow(10, gain_db / 20);

  T b0_peq = (1 + V * bp_coefs.b0) / (1 + bp_coefs.a1 + bp_coefs.a2);
  T b1_peq = (bp_coefs.b1) / (1 + bp_coefs.a1 + bp_coefs.a2);
  T b2_peq = (1 + V * bp_coefs.b2) / (1 + bp_coefs.a1 + bp_coefs.a2);
  T a1_peq = (bp_coefs.a1) / (1 + bp_coefs.a1 + bp_coefs.a2);
  T a2_peq = (bp_coefs.a2) / (1 + bp_coefs.a1 + bp_coefs.a2);

  return IIRFilterCoefficients<T>{1.0, b0_peq, b1_peq, b2_peq, a1_peq, a2_peq};
}