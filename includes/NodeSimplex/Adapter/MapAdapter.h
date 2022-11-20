#pragma once

#include <vector>


namespace Clover::NodeSimplex::Adapter {

typedef std::tuple<int, int> Mapping;
struct MapAdapterSettings
{
  MapAdapterSettings() : mappings() {}
  std::vector<Mapping> mappings;
};

template <size_t __arityIn, size_t __arityOut>
class MapAdapter : public StatefulProcessor<__arityIn, __arityOut, MapAdapterSettings>
{
public:
  MapAdapter() {}
  MapAdapter(MapAdapterSettings initialSettings)
  : StatefulProcessor<__arityIn, __arityOut, MapAdapterSettings>(initialSettings)
  {
    std::for_each(
      initialSettings.mappings.cbegin(),
      initialSettings.mappings.cend(),
      [=, this](Mapping m){
        this->validateMapping(m);
      }
    );
  }

  /// BEWARE: there are no safeguards in place to prevent inefficient mappings or overwriting previously registered mappings
  void map(int from, int to)
  {
    validateMapping(from, to);
    this->settings.current.mappings.emplace_back(Mapping {from, to});
  }

  /// BEWARE: there are no safeguards in place to prevent inefficient mappings or overwriting previously registered mappings
  void mapRange(int from, int count, int to)
  {
    validateMapping(from, count, to);
    for(int i = 0; i < count; i++)
    {
      map(from + i, to + i);
    }
  }

  void validateMapping(Mapping mapping)
  {
      validateMapping(std::get<0>(mapping), std::get<1>(mapping));
  }

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

private:
  Frame<__arityOut> tick(Frame<__arityIn> input)
  {
    Frame<__arityOut> f {};

    for ( int i = 0, end = this->settings.current.mappings.size(); i < end; i++)
    {
      int from = std::get<0>(this->settings.current.mappings.at(i));
      int to =   std::get<1>(this->settings.current.mappings.at(i));

      f[to] = input[from];
    }

    return f;
  }
};

}