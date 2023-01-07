#pragma once

#include "Graph.h"
#include "IO.h"

namespace Clover::Graph {

template <size_t __arityIn, size_t __arityOut, class SettingsType>
class StatefulSubgraph
    : public StatefulProcessor<__arityIn, __arityOut, SettingsType> {
public:
  StatefulSubgraph()
      : StatefulProcessor<__arityIn, __arityOut, SettingsType>() {}

  StatefulSubgraph(SettingsType initialSettings)
      : StatefulProcessor<__arityIn, __arityOut, SettingsType>(
            initialSettings) {}

protected:
  Clover::IO::RootNode<__arityOut> blackHole;

  void tickCallback(int currentClockTime) override {
    blackHole.metaTick(currentClockTime);
  }
};

} // namespace Clover::Graph