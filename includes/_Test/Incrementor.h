#pragma once

#include "Graph.h"

using namespace Clover::Graph;

namespace Clover::_Test {

template <size_t __arityOut> class Incrementor : public Node<0, __arityOut> {
public:
  Incrementor(int startValue = 0) : Node<0, __arityOut>(), basis(startValue) {}

private:
  float basis;
  Frame<__arityOut> tick(Frame<0> input) {
    Frame<__arityOut> f{};

    float time = (float)this->_currentClockTime;
    for (int i = 0; i < __arityOut; i++) {
      f[i] = time + basis;
    }

    return f;
  }
};

} // namespace Clover::_Test