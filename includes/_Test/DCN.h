#pragma once

#include "Graph/AudioNode.h"

using namespace Clover::Graph;
using namespace Clover::Util;

namespace Clover::_Test {

template <size_t __arityOut> class DCN : public AudioNode<0, __arityOut> {
public:
  DCN() : AudioNode<0, __arityOut>(), basis(0.f) {}

  void indexBasis(float b) { basis = b; }

private:
  float basis;
  AudioFrame<__arityOut> tick(AudioFrame<0> input) {
    AudioFrame<__arityOut> f{};

    for (int i = 0; i < __arityOut; i++) {
      f[i] = i + basis;
    }

    return f;
  }
};

} // namespace Clover::_Test