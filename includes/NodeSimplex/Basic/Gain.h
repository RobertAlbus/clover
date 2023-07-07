#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Basic {

template <size_t __arity>
class Gain : public Graph::AudioNode<__arity, __arity> {

  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    return input;
  };
};

} // namespace Clover::NodeSimplex::Basic
