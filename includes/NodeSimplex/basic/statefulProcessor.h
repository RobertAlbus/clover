#pragma once

#include <functional>

#include "graph.h"

using namespace Clover::Graph;

namespace Clover::Graph
{

template <class SettingsType>
struct NodeSettings
{
  SettingsType current;
  SettingsType initial;

  void reset()
  {
    current = initial;
  }
};

/// A Node with a transform function that does not maintain any inter-sample state.
template <size_t __arityIn, size_t __arityOut, class SettingsType> 
class StatefulProcessor : public Node<__arityIn, __arityOut>
{
public:
  StatefulProcessor() : Node<__arityIn, __arityOut>() {}
  NodeSettings<SettingsType> settings;
};

}
