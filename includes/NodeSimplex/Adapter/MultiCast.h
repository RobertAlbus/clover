#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Adapter {

template <size_t __arityOut> class MultiCast : public AudioNode<1, __arityOut> {

  AudioFrame<__arityOut> tick(AudioFrame<1> input) {
    AudioFrame<__arityOut> f{};

    for (int i = 0; i < __arityOut; i++) {
      f[i] = input[0];
    }

    return f;
  }
};

} // namespace Clover::NodeSimplex::Adapter