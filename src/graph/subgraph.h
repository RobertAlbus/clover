#pragma once

#include "node.h"
#include "rootNode.h"

template <size_t __arityIn, size_t __arityOut>
class Subgraph : public Node<__arityIn, __arityOut>
{
protected:
  RootNode<__arityOut> blackHole;

  void _tickCallback(int currentClockTime) override
  {
    blackHole._tick(currentClockTime);
  }
};