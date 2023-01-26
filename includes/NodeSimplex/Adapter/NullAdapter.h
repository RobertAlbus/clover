#pragma once

namespace Clover::NodeSimplex::Adapter {

template <size_t __arityIn, size_t __arityOut>
class NullAdapter : public StatelessProcessorV2<__arityIn, __arityOut> {
  Frame<__arityOut> processFn(Frame<__arityIn> input) {
    return Frame<__arityOut>{};
  }
};

} // namespace Clover::NodeSimplex::Adapter