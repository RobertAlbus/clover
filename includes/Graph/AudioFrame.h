#pragma once

#include <array>
#include <cstdio>

#include "Constants.h"

namespace Clover::Graph {

template <size_t __arity> struct AudioFrame {

  AudioFrame() : data() {}

  AudioFrame(std::initializer_list<Sample> init) {
    if (init.size() != __arity) {
      printf("\nCannot initialize AudioFrame<%zu> with %zu elements\n", __arity,
             init.size());
    }
    std::copy(init.begin(), init.end(), data.begin());
  }

  float &operator[](int i) { return data[i]; }

  float operator[](int i) const { return data[i]; }

  AudioFrame<__arity> operator+(const AudioFrame<__arity> &b) const {
    AudioFrame<__arity> f;
    for (int i = 0; i < __arity; i++) {
      f[i] = data[i] + b[i];
    }
    return f;
  }

  AudioFrame<__arity> operator-(const AudioFrame<__arity> &b) const {
    AudioFrame<__arity> f;
    for (int i = 0; i < __arity; i++) {
      f[i] = data[i] - b[i];
    }
    return f;
  }

  AudioFrame<__arity> operator*(const AudioFrame<__arity> &b) const {
    AudioFrame<__arity> f;
    for (int i = 0; i < __arity; i++) {
      f[i] = data[i] * b[i];
    }
    return f;
  }

  AudioFrame<__arity> operator/(const AudioFrame<__arity> &b) const {
    AudioFrame<__arity> f;
    for (int i = 0; i < __arity; i++) {
      f[i] = data[i] / b[i];
    }
    return f;
  }

  void operator+=(const AudioFrame<__arity> &b) {
    for (int i = 0; i < __arity; i++) {
      data[i] += b[i];
    }
  }

  void operator-=(const AudioFrame<__arity> &b) {
    for (int i = 0; i < __arity; i++) {
      data[i] -= b[i];
    }
  }

  void operator*=(const AudioFrame<__arity> &b) {
    for (int i = 0; i < __arity; i++) {
      data[i] *= b[i];
    }
  }

  void operator/=(const AudioFrame<__arity> &b) {
    for (int i = 0; i < __arity; i++) {
      data[i] /= b[i];
    }
  }

  void operator+=(const float b) {
    for (int i = 0; i < __arity; i++) {
      data[i] += b;
    }
  }

  void operator-=(const float b) {
    for (int i = 0; i < __arity; i++) {
      data[i] -= b;
    }
  }

  void operator*=(const float b) {
    for (int i = 0; i < __arity; i++) {
      data[i] *= b;
    }
  }

  void operator/=(const float b) {
    for (int i = 0; i < __arity; i++) {
      data[i] /= b;
    }
  }

  void init() {
    for (int i = 0; i < __arity; i++) {
      data[i] = 0;
    }
  }

  std::array<Sample, __arity> data;
};

} // namespace Clover::Graph