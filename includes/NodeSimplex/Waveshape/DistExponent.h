#pragma once

#include <cmath>

#include "Graph.h"

using namespace Clover::Graph;
using namespace Clover::Util;

namespace Clover::NodeSimplex::Waveshape {

template <size_t __arity> class DistExponent : public AudioNode<__arity, __arity> {
public:
  DistExponent() : AudioNode<__arity, __arity>(), _exponent(1) {}

  void exponent(float e) { _exponent = e; }
  float exponent() { return _exponent; }

private:
  float _exponent;
  Frame<__arity> tick(Frame<__arity> input) {
    Frame<__arity> f{};
    for (int i = 0; i < __arity; i++) {
      float sign = Calc::sign(input[i]);
      float abs = fabs(input[i]);
      f[i] = pow(abs, 1.f - _exponent) * sign;
    }

    return f;
  }
};

} // namespace Clover::NodeSimplex::Waveshape
