#pragma once

#include <math.h>

#include "Graph.h"
#include "NodeSimplex.h"

namespace Clover::NodeSimplex::Basic {

template <size_t __arity> class Rectifier : public AudioNode<__arity, __arity> {

  Frame<__arity> tick(Frame<__arity> input) {
    Frame<__arity> f;
    for (int i = 0; i < __arity; i++) {
      f[i] = fabs(input[i]);
    }

    return f;
  }
};

} // namespace Clover::NodeSimplex::Basic