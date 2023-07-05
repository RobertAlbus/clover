#pragma once

#include <functional>

#include "Graph.h"

namespace Clover::Graph {

template <class SettingsType> struct NodeSettings {
  NodeSettings() : current(), initial() {}
  NodeSettings(const SettingsType &settings)
      : current(settings), initial(settings) {}
  SettingsType current;
  SettingsType initial;

  void reset() { current = initial; }
};

/// A Node with a transform function that maintains inter-sample state.
template <size_t __arityIn, size_t __arityOut, class SettingsType>
class StatefulProcessor : public Graph::AudioNode<__arityIn, __arityOut> {
public:
  StatefulProcessor() : Graph::AudioNode<__arityIn, __arityOut>() {}
  StatefulProcessor(const SettingsType &initialSettings)
      : Graph::AudioNode<__arityIn, __arityOut>(), settings(initialSettings) {
    settings.reset();
  }

  NodeSettings<SettingsType> settings;
};

} // namespace Clover::Graph
