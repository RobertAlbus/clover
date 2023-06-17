#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Adapter {

template <size_t __arityOut> class MultiCast : public AudioNode<1, __arityOut> {

  Frame<__arityOut> tick(Frame<1> input) {
    Frame<__arityOut> f{};

    for (int i = 0; i < __arityOut; i++) {
      f[i] = input[0];
    }

    return f;
  }
};

} // namespace Clover::NodeSimplex::Adapter