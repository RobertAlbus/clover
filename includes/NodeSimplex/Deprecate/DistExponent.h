#pragma once

#include <cmath>

#include "Graph.h"

namespace Clover::NodeSimplex::Waveshape {

template <size_t __arity>
class DistExponent : public Graph::AudioNode<__arity, __arity> {
public:
  DistExponent() : Graph::AudioNode<__arity, __arity>(), _exponent(1) {}

  void exponent(float e) { _exponent = e; }
  float exponent() { return _exponent; }

private:
  float _exponent;
  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    Graph::AudioFrame<__arity> f{};
    for (int i = 0; i < __arity; i++) {
      float sign = Util::Calc::sign(input[i]);
      float abs = fabs(input[i]);
      f[i] = pow(abs, 1.f - _exponent) * sign;
    }

    return f;
  }
};

} // namespace Clover::NodeSimplex::Waveshape
