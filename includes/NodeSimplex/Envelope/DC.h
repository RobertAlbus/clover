#pragma once

#include "Algo/Envelope/DC.h"
#include "Graph.h"

namespace Clover::NodeSimplex::Envelope {

class DC : public Graph::AudioNode<0, 1> {
public:
  DC(Sample value = 0);

  void value(Sample v);
  Sample value();

protected:
  Graph::AudioFrame<1> tick(Graph::AudioFrame<0> input);
  Clover::Envelope::DC<Sample> dc;
};

} // namespace Clover::NodeSimplex::Envelope