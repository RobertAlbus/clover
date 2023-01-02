#pragma once

#include "Graph.h"

using namespace Clover::Graph;
using namespace Clover::Util;

namespace Clover::_Test {

template <size_t __arityOut> class DCN : public Node<0, __arityOut> {
public:
  DCN() : Node<0, __arityOut>() {}

private:
  Frame<__arityOut> tick(Frame<0> input) {
    Frame<__arityOut> f{};

    for (int i = 0; i < __arityOut; i++) {
      f[i] = i;
    }

    return f;
  }
};

} // namespace Clover::_Test