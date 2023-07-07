#pragma once

#include "Graph/AudioNode.h"

namespace Clover::_Test {

template <size_t __arityOut>
class DCN : public Graph::AudioNode<0, __arityOut> {
public:
  DCN() : Graph::AudioNode<0, __arityOut>(), basis(0.f) {}

  void indexBasis(float b) { basis = b; }

private:
  float basis;
  Graph::AudioFrame<__arityOut> tick(Graph::AudioFrame<0> input) {
    Graph::AudioFrame<__arityOut> f{};

    for (int i = 0; i < __arityOut; i++) {
      f[i] = i + basis;
    }

    return f;
  }
};

} // namespace Clover::_Test