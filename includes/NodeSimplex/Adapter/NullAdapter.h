#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Adapter {

template <size_t __arityIn, size_t __arityOut>
class NullAdapter : public AudioNode<__arityIn, __arityOut> {

  Frame<__arityOut> tick(Frame<__arityIn> input) { return Frame<__arityOut>{}; }
};

} // namespace Clover::NodeSimplex::Adapter