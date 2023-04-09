#pragma once

#include "Util.h"

template <FloatingPoint T, int __arity> class JilesAthertonModel {
public:
  JilesAthertonModel(T sampleRate) { initializeDefaultParameters(sampleRate); }

  std::array<T, __arity>
  processSample(const std::array<T, __arity> &inputValues) {
    updateStateVariables(inputValues);
    return computeOutput();
  }

  void reset() { stateVariables = initialState; }

private:
  std::array<T, __arity> stateVariables;
  std::array<T, __arity> initialState;

  // Parameters and constants for the Jiles-Atherton model
  T alpha, a, k, c, Ms, dt, sampleRate;

  void initializeDefaultParameters(T sampleRate) {
    alpha = static_cast<T>(0.001);
    a = static_cast<T>(25);
    k = static_cast<T>(100);
    c = static_cast<T>(0.8);
    Ms = static_cast<T>(1.0);
    sampleRate = sampleRate;
    dt = static_cast<T>(1.0 / sampleRate); // Time step for sample rate

    initialState.fill(static_cast<T>(0.0));
    stateVariables = initialState;
  }

  void updateStateVariables(const std::array<T, __arity> &inputValues) {
    for (std::size_t i = 0; i < __arity; i++) {
      T H = inputValues[i];
      T m = stateVariables[i];

      // 4th order Runge-Kutta integration
      T k1 = dt * dmdt(H, m);
      T k2 = dt * dmdt(H, m + k1 / static_cast<T>(2));
      T k3 = dt * dmdt(H, m + k2 / static_cast<T>(2));
      T k4 = dt * dmdt(H, m + k3);

      stateVariables[i] +=
          (k1 + static_cast<T>(2) * k2 + static_cast<T>(2) * k3 + k4) /
          static_cast<T>(6);
    }
  }

  std::array<T, __arity> computeOutput() {
    std::array<T, __arity> output;
    for (std::size_t i = 0; i < __arity; i++) {
      T H = stateVariables[i];
      T H_eff = computeEffectiveField(H);
      T mH =
          stateVariables[0] / Ms; // Compute the normalized magnetization m(H)
      output[i] = mH * H_eff;
    }
    return output;
  }

  T computeEffectiveField(T H) {
    T dm_dt = dmdt(H, stateVariables[0]);
    return H - c * dm_dt - k * stateVariables[0];
  }

  T dmdt(T H, T m) {
    T M_H = Ms * m;
    T H_eff = H - c * (M_H - m * Ms) / (a * alpha) - k * m;
    return ((1 - m * M_H) / (a * H_eff)) -
           alpha * m * (M_H - m * Ms) / (a * H_eff * H_eff);
  }
};
