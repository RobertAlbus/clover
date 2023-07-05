#pragma once

#include "Graph.h"



namespace Clover::_Test {

template <size_t __arityOut>
class Incrementor : public Graph::AudioNode<0, __arityOut> {
public:
  Incrementor(int startValue = 0)
      : Graph::AudioNode<0, __arityOut>(), basis(startValue) {}

private:
  float basis;
  Graph::AudioFrame<__arityOut> tick(Graph::AudioFrame<0> input) {
    Graph::AudioFrame<__arityOut> f{};

    float time = (float)this->_currentClockTime;
    for (int i = 0; i < __arityOut; i++) {
      f[i] = time + basis;
    }

    return f;
  }
};

} // namespace Clover::_Test