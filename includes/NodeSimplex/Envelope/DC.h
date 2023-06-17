#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Envelope {

struct DCSettings {
  DCSettings(Sample value = 0);

  Sample value;
};

class DC : public StatefulProcessor<0, 1, DCSettings> {
public:
  DC(Sample value = 0);
  DC(DCSettings initialSettings);

  void value(Sample v);
  Sample value();

protected:
  AudioFrame<1> tick(AudioFrame<0> input);
};

} // namespace Clover::NodeSimplex::Envelope