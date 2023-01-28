#include "Graph.h"
#include "NodeSimplex/Envelope/DC.h"

namespace Clover::NodeSimplex::Envelope {

DCSettings::DCSettings(Sample value) : value(value) {}

DC::DC(Sample value) : DC(*(new DCSettings(value))) {}
DC::DC(DCSettings initialSettings) : StatefulProcessor(initialSettings) {}

void DC::value(Sample v) { settings.current.value = v; }
Sample DC::value() { return settings.current.value; }

Frame<1> DC::tick(Frame<0> input) {
  return Frame<1> {settings.current.value};
}

} // namespace Clover::NodeSimplex::Envelope