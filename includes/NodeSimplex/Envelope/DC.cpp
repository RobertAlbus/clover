#include "Graph.h"
#include "NodeSimplex/Envelope/DC.h"

namespace Clover::NodeSimplex::Envelope {

DC::DC(Sample value) : dc(value) {}

void DC::value(Sample v) { dc.value(v); }
Sample DC::value() { return dc.last(); }

Graph::AudioFrame<1> DC::tick(Graph::AudioFrame<0> input) {
  return Graph::AudioFrame<1> {dc.process()};
}

} // namespace Clover::NodeSimplex::Envelope