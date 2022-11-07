#pragma once

#include <vector>

#include "graph/frame.h"

#include "NodeSimplex.h"

using namespace Clover::Graph;

namespace Clover::NodeSimplex::Adapter {

/// BEWARE: there are no safeguards in place to prevent inefficient mappings or overwriting previously registered mappings
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

  void mapRange(int from, int count, int to)
  {
    validateMapping(from, count, to);
    for(int i = 0; i < count; i++)
    {
      map(from + i, to + i);
    }
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

  void validateMapping(int from, int count, int to)
  {
    bool rangeExceedsInputArity  = count + from > __arityIn;
    bool rangeExceedsOutputArity = count + to   > __arityOut;
    if (rangeExceedsInputArity || rangeExceedsOutputArity) 
    {
      printf("\n\ninvalid range mapping in MapAdapter<%i,%i>", __arityIn, __arityOut);
      if(rangeExceedsInputArity)
        printf("\n  mapping range of size %i exceeds the capacity of the input frame.", count);
      if(rangeExceedsOutputArity)
        printf("\n  mapping range of size %i exceeds the capacity of the output frame.", count);
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