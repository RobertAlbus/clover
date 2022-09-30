#pragma once

#include "node.h"
#include "rootNode.h"

template <size_t __arityIn, size_t __arityOut>
class Subgraph : public Node<__arityIn, __arityOut>
{
protected:
  Clover::IO::RootNode<__arityOut> blackHole;

  void tickCallback(int currentClockTime) override
  {
    blackHole.metaTick(currentClockTime);
  }
};