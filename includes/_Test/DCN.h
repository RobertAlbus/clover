#pragma once

#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"
#include "Graph/NullFrame.h"

namespace Clover::_Test {

template <size_t __arityOut>
class DCN : public Graph::AudioOutNode<__arityOut> {
public:
  DCN() : Graph::AudioOutNode<__arityOut>(), basis(0.f) {}

  void indexBasis(float b) { basis = b; }

private:
  float basis;
  Graph::AudioFrame<__arityOut> tick(Graph::NullFrame input) {
    Graph::AudioFrame<__arityOut> f{};

    for (int i = 0; i < __arityOut; i++) {
      f[i] = i + basis;
    }

    return f;
  }
};

} // namespace Clover::_Test