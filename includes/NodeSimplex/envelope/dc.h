#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Envelope {

struct DCSettings
{
  DCSettings() : DCSettings(0) {}
  DCSettings(Sample value) :value(value) {}

  Sample value;
};

template <size_t __arity>
class DC : public StatefulProcessor<1, 1, DCSettings>
{
public:
  DC() : DC(0) {}
  DC(Sample value) : DC(*(new DCSettings(value))) {}
  DC(DCSettings initialSettings)
  : StatefulProcessor(initialSettings) {}

  void   value(Sample v) { settings.current.value = v; }
  Sample value()         { return settings.current.value; }

protected:
  Frame<__arity> tick(Frame<__arity> input)
  {
    Frame<__arity> f {};

    for (int i = 0; i < __arity; i++)
    {
      f[i] = settings.current.value + input[i];
    }

    return f;
  }
};

}