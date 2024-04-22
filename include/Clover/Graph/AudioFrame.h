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
#include <cstdio>

#include "Clover/Constants.h"

namespace Clover::Graph {

template <size_t __arity> struct AudioFrame {

  AudioFrame() : data() {}

  AudioFrame(std::array<Sample, __arity> array) : data(array) {}

  AudioFrame(std::initializer_list<Sample> init) {
    if (init.size() != __arity) {
      printf(
          "\nCannot initialize Graph::AudioFrame<%zu> with %zu elements\n",
          __arity,
          init.size()
      );
    }
    std::copy(init.begin(), init.end(), data.begin());
  }

  float &operator[](int i) { return data[static_cast<size_t>(i)]; }

  float operator[](int i) const { return data[static_cast<size_t>(i)]; }

  Graph::AudioFrame<__arity> operator+(const Graph::AudioFrame<__arity> &b
  ) const {
    Graph::AudioFrame<__arity> f;
    for (size_t i = 0; i < __arity; i++) {
      f[i] = data[i] + b[i];
    }
    return f;
  }

  Graph::AudioFrame<__arity> operator-(const Graph::AudioFrame<__arity> &b
  ) const {
    Graph::AudioFrame<__arity> f;
    for (size_t i = 0; i < __arity; i++) {
      f[i] = data[i] - b[i];
    }
    return f;
  }

  Graph::AudioFrame<__arity> operator*(const Graph::AudioFrame<__arity> &b
  ) const {
    Graph::AudioFrame<__arity> f;
    for (size_t i = 0; i < __arity; i++) {
      f[i] = data[i] * b[i];
    }
    return f;
  }

  Graph::AudioFrame<__arity> operator/(const Graph::AudioFrame<__arity> &b
  ) const {
    Graph::AudioFrame<__arity> f;
    for (size_t i = 0; i < __arity; i++) {
      f[i] = data[i] / b[i];
    }
    return f;
  }

  void operator+=(const Graph::AudioFrame<__arity> &b) {
    for (size_t i = 0; i < __arity; i++) {
      data[i] += b[i];
    }
  }

  void operator-=(const Graph::AudioFrame<__arity> &b) {
    for (size_t i = 0; i < __arity; i++) {
      data[i] -= b[i];
    }
  }

  void operator*=(const Graph::AudioFrame<__arity> &b) {
    for (size_t i = 0; i < __arity; i++) {
      data[i] *= b[i];
    }
  }

  void operator/=(const Graph::AudioFrame<__arity> &b) {
    for (size_t i = 0; i < __arity; i++) {
      data[i] /= b[i];
    }
  }

  void operator+=(const float b) {
    for (size_t i = 0; i < __arity; i++) {
      data[i] += b;
    }
  }

  void operator-=(const float b) {
    for (size_t i = 0; i < __arity; i++) {
      data[i] -= b;
    }
  }

  void operator*=(const float b) {
    for (size_t i = 0; i < __arity; i++) {
      data[i] *= b;
    }
  }

  void operator/=(const float b) {
    for (size_t i = 0; i < __arity; i++) {
      data[i] /= b;
    }
  }

  void init() {
    for (size_t i = 0; i < __arity; i++) {
      data[i] = 0;
    }
  }

  std::array<Sample, __arity> data;
};

} // namespace Clover::Graph
