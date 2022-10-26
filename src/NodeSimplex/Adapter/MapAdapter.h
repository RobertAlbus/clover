#pragma once

#include <vector>

#include "graph/frame.h"

#include "../nodeSimplex.h"

using namespace Clover::Graph;

namespace Clover::NodeSimplex::Adapter {

template <size_t __arityIn, size_t __arityOut>
class MapAdapter : public Node<__arityIn, __arityOut>
{
public:
  MapAdapter() : 
    Node<__arityIn, __arityOut>()
  {

  }

  void map(int from, int to)
  {
    validateMapping(from, to);
    mappings.emplace_back(std::tuple<int, int> {from, to});
  }

private:
  std::vector<std::tuple<int, int>> mappings;

  void validateMapping(int from, int to)
  {
    bool isFromValid = from < __arityIn;
    bool isToValid   = to   < __arityOut;
    if ( !isFromValid || !isToValid ) 
    {
      printf("\n\ninvalid mapping in MapAdapter<%i,%i>: %i to %i.", __arityIn, __arityOut, from, to);
      if(!isFromValid) printf("\n  %i is out of range for the input frame .", from);
      if(!isToValid) printf("\n  %i is out of range for the output frame .", to);
      exit(EXIT_FAILURE);
    }
  }
  Frame<__arityOut> tick(Frame<__arityIn> input)
  {
    Frame<__arityOut> f {};

    for ( int i = 0, end = mappings.size(); i < end; i++)
    {
      int from = std::get<0>(mappings.at(i));
      int to =   std::get<1>(mappings.at(i));

      f[to] = input[from];
    }

    return f;
  }
};

}