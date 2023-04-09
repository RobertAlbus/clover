#include <cmath>

#include "IIR_Coefficients.h"
#include "Util.h"

template <FloatingPoint T> T calculateOmegaC(T cutoff_hz, T samplerate_hz) {
  return std::tan(M_PI * cutoff_hz / samplerate_hz);
}

template <FloatingPoint T> IIRFilterCoefficients<T> butterLowPass(T cutoff_hz, T Q, T samplerate_hz) {
  T omega_c = calculateOmegaC(cutoff_hz, samplerate_hz);
  T alpha = omega_c / Q;
  T denominator = T(1) + alpha;

  T b0 = omega_c / (T(2) * denominator);
  T b1 = omega_c / denominator;
  T b2 = b0;
  T a1 = (T(2) * (omega_c * omega_c - T(1))) / denominator;
  T a2 = (T(1) - alpha) / denominator;

  return IIRFilterCoefficients<T>{T(1), b0, b1, b2, a1, a2};
}

template <FloatingPoint T> IIRFilterCoefficients<T> butterHighPass(T cutoff_hz, T Q, T samplerate_hz) {
  T omega_c = calculateOmegaC(cutoff_hz, samplerate_hz);
  T alpha = omega_c / Q;
  T denominator = T(1) + alpha;

  T b0 = T(1) / (T(2) * denominator);
  T b1 = -T(2) * b0;
  T b2 = b0;
  T a1 = (T(2) * (omega_c * omega_c - T(1))) / denominator;
  T a2 = (T(1) - alpha) / denominator;

  return IIRFilterCoefficients<T>{T(1), b0, b1, b2, a1, a2};
}

template <FloatingPoint T> IIRFilterCoefficients<T> butterBandPass(T cutoff_hz, T Q, T samplerate_hz) {
  T omega_c = calculateOmegaC(cutoff_hz, samplerate_hz);
  T alpha = omega_c / Q;
  T denominator = T(1) + alpha;

  T b0 = alpha / denominator;
  T b1 = T(0);
  T b2 = -b0;
  T a1 = (T(2) * (omega_c * omega_c - T(1))) / denominator;
  T a2 = (T(1) - alpha) / denominator;

  return IIRFilterCoefficients<T>{T(1), b0, b1, b2, a1, a2};
}

template <FloatingPoint T> IIRFilterCoefficients<T> butterNotch(T cutoff_hz, T Q, T samplerate_hz) {
  T omega_c = calculateOmegaC(cutoff_hz, samplerate_hz);
  T alpha = omega_c / Q;
  T denominator = T(1) + alpha;

  T b0 = T(1);
  T b1 = (T(2) * (omega_c * omega_c - T(1))) / denominator;
  T b2 = (T(1) - alpha) / denominator;
  T a1 = b1;
  T a2 = b2;

  return IIRFilterCoefficients<T>{T(1), b0, b1, b2, a1, a2};
}

template <FloatingPoint T> IIRFilterCoefficients<T> butterLowShelf(T cutoff_hz, T Q, T gain_db, T samplerate_hz) {
  T A = std::pow(T(10), gain_db / T(40));
  T omega_c = calculateOmegaC(cutoff_hz, samplerate_hz);
  T beta = std::sqrt(A) / Q;
  T denominator = T(1) + omega_c / beta + omega_c * omega_c;

  T b0 = A * (T(1) + omega_c / beta + omega_c * omega_c) / denominator;
  T b1 = T(2) * A * (omega_c * omega_c - T(1)) / denominator;
  T b2 = A * (T(1) - omega_c / beta + omega_c * omega_c) / denominator;
  T a1 = T(2) * (omega_c * omega_c - T(1)) / denominator;
  T a2 = (T(1) - omega_c / beta + omega_c * omega_c) / denominator;

  return IIRFilterCoefficients<T>{T(1), b0, b1, b2, a1, a2};
}

template <FloatingPoint T> IIRFilterCoefficients<T> butterHighShelf(T cutoff_hz, T Q, T gain_db, T samplerate_hz) {
  T A = std::pow(T(10), gain_db / T(40));
  T omega_c = calculateOmegaC(cutoff_hz, samplerate_hz);
  T beta = std::sqrt(A) / Q;
  T denominator = T(1) + omega_c / beta + omega_c * omega_c;

  T b0 = A * (T(1) - omega_c / beta + omega_c * omega_c) / denominator;
  T b1 = T(2) * A * (T(1) - omega_c * omega_c) / denominator;
  T b2 = A * (T(1) + omega_c / beta + omega_c * omega_c) / denominator;
  T a1 = T(2) * (omega_c * omega_c - T(1)) / denominator;
  T a2 = (T(1) - omega_c / beta + omega_c * omega_c) / denominator;

  return IIRFilterCoefficients<T>{T(1), b0, b1, b2, a1, a2};
}

template <FloatingPoint T> IIRFilterCoefficients<T> butterpeakingEQ(T center_freq_hz, T Q, T gain_db, T samplerate_hz) {
  T A = std::pow(T(10), gain_db / T(40));
  T omega_c = calculateOmegaC(center_freq_hz, samplerate_hz);
  T sin_omega_c = std::sin(omega_c);
  T alpha = sin_omega_c / (T(2) * Q);

  T denominator = T(1) + alpha / A;

  T b0 = (T(1) + alpha * A) / denominator;
  T b1 = T(-2) * std::cos(omega_c) / denominator;
  T b2 = (T(1) - alpha * A) / denominator;
  T a1 = T(2) * std::cos(omega_c) / denominator;
  T a2 = (T(1) - alpha / A) / denominator;

  return IIRFilterCoefficients<T>{T(1), b0, b1, b2, a1, a2};
}
