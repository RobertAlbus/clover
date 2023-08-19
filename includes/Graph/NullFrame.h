#pragma once

#include <array>
#include <cstdio>

#include "Constants.h"

namespace Clover::Graph {

struct NullFrame {

  NullFrame() {}
  NullFrame(std::initializer_list<Sample> init) {}

  NullFrame operator+(const Graph::NullFrame &b) const { return b; }

  NullFrame operator-(const Graph::NullFrame &b) const { return b; }

  Graph::NullFrame operator*(const Graph::NullFrame &b) const { return b; }

  Graph::NullFrame operator/(const Graph::NullFrame &b) const { return b; }

  void operator+=(const Graph::NullFrame &b) {}

  void operator-=(const Graph::NullFrame &b) {}

  void operator*=(const Graph::NullFrame &b) {}

  void operator/=(const Graph::NullFrame &b) {}

  void operator+=(const float b) {}

  void operator-=(const float b) {}

  void operator*=(const float b) {}

  void operator/=(const float b) {}

  void init() {}
};

} // namespace Clover::Graph