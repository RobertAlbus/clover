#pragma once

#include <cmath>

#include "Graph.h"

namespace Clover::NodeSimplex::Waveshape {

template <size_t __arity> class RingMod_V1 : public AudioNode<__arity, __arity> {
public:
  RingMod_V1() : AudioNode<__arity, __arity>(), _shape(0) {}

  void shape(float s) { _shape = s; }
  float shape() { return _shape; }

private:
  float _shape;
  AudioFrame<__arity> tick(AudioFrame<__arity> input) {
    float shape = ((_shape * 0.5) + 0.5) * M_PI;
    AudioFrame<__arity> f{};
    for (int i = 0; i < __arity; i++) {
      f[i] = sin(shape * input[i]);
    }

    return f;
  }
};

} // namespace Clover::NodeSimplex::Waveshape