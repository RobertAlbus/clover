#pragma once

#include "Graph/AudioNode.h"
#include "IO/RootNode.h"

template <size_t __arityIn, size_t __arityOut>
class Subgraph : public Clover::Graph::AudioNode<__arityIn, __arityOut> {
protected:
  Clover::IO::RootNode<__arityOut> blackHole;

  void tickCallback(int currentClockTime) override {
    blackHole.metaTick(currentClockTime);
  }
};