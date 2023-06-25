#pragma once

#include "Algo/Envelope/DC.h"
#include "Graph.h"

namespace Clover::NodeSimplex::Envelope {

class DC : public AudioNode<0, 1> {
public:
  DC(Sample value = 0);

  void value(Sample v);
  Sample value();

protected:
  AudioFrame<1> tick(AudioFrame<0> input);
  Clover::Envelope::DC<Sample> dc;
};

} // namespace Clover::NodeSimplex::Envelope