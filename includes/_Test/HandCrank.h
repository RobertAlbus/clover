#pragma once

#include "Graph.h"

using namespace Clover::Graph;
using namespace Clover::Util;

namespace Clover::_Test {

template <size_t __arityIn> class HandCrank : public Node<__arityIn, 0> {
public:
  HandCrank() : Node<__arityIn, 0>() {}

  void turn(int numberOfTicks) {
    for (int i = 0; i < numberOfTicks; i++) {
      this->metaTick(clock.currentSample());
      clock.tick();
    }
  }

private:
  Clover::Util::SampleClock clock;

  Frame<0> tick(Frame<__arityIn> input) { return Frame<0>{}; }
};

} // namespace Clover::_Test