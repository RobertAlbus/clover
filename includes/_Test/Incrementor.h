#pragma once

#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"
#include "Graph/NullFrame.h"

namespace Clover::_Test {

template <size_t __arityOut>
class Incrementor : public Graph::AudioOutNode<__arityOut> {
public:
  Incrementor(int startValue = 0)
      : Graph::AudioOutNode<__arityOut>(), basis(startValue) {}

private:
  float basis;
  Graph::AudioFrame<__arityOut> tick(Graph::NullFrame input) {
    Graph::AudioFrame<__arityOut> f{};

    float time = (float)this->_currentClockTime;
    for (int i = 0; i < __arityOut; i++) {
      f[i] = time + basis;
    }

    return f;
  }
};

} // namespace Clover::_Test