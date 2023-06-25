#include "Graph.h"
#include "NodeSimplex/Envelope/DC.h"

namespace Clover::NodeSimplex::Envelope {

DC::DC(Sample value) : dc(value) {}

void DC::value(Sample v) { dc.value(v); }
Sample DC::value() { return dc.last(); }

AudioFrame<1> DC::tick(AudioFrame<0> input) {
  return AudioFrame<1> {dc.process()};
}

} // namespace Clover::NodeSimplex::Envelope