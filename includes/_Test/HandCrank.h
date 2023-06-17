#pragma once

#include "Graph.h"

using namespace Clover::Graph;
using namespace Clover::Util;

namespace Clover::_Test {

template <size_t __arityIn> class HandCrank : public AudioNode<__arityIn, 0> {
public:
  HandCrank() : AudioNode<__arityIn, 0>() {}

  void turn(int numberOfTicks) {
    for (int i = 0; i < numberOfTicks; i++) {
      this->metaTick(clock.currentSample());
      clock.tick();
    }
  }

private:
  Clover::Util::SampleClock clock;

  AudioFrame<0> tick(AudioFrame<__arityIn> input) { return AudioFrame<0>{}; }
};

} // namespace Clover::_Test