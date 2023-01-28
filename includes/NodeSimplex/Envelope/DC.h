#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Envelope {

struct DCSettings {
  DCSettings(Sample value = 0) : value(value) {}

  Sample value;
};

class DC : public StatefulProcessor<0, 1, DCSettings> {
public:
  DC(Sample value = 0) : DC(*(new DCSettings(value))) {}
  DC(DCSettings initialSettings) : StatefulProcessor(initialSettings) {}

  void value(Sample v) { settings.current.value = v; }
  Sample value() { return settings.current.value; }

protected:
  Frame<1> tick(Frame<0> input) {
    Frame<1> f{};

    f[0] = settings.current.value;

    return f;
  }
};

} // namespace Clover::NodeSimplex::Envelope