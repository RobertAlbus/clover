#pragma once

#include "Graph/NullFrame.h"
#include "Graph/NullNode.h"
#include "Util/SampleClock.h"

namespace Clover::_Test {

class HandCrank : public Graph::NullNode {
public:
  HandCrank() : Graph::NullNode() {}

  void turn(int numberOfTicks) {
    for (int i = 0; i < numberOfTicks; i++) {
      this->metaTick(clock.currentSample());
      clock.tick();
    }
  }

private:
  Clover::Util::SampleClock clock;

  Graph::NullFrame tick(Graph::NullFrame input) { return Graph::NullFrame{}; }
};

} // namespace Clover::_Test