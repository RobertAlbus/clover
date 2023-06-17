#include "Graph.h"
#include "NodeSimplex/Envelope/DC.h"

namespace Clover::NodeSimplex::Envelope {

DCSettings::DCSettings(Sample value) : value(value) {}

DC::DC(Sample value) : DC(*(new DCSettings(value))) {}
DC::DC(DCSettings initialSettings) : StatefulProcessor(initialSettings) {}

void DC::value(Sample v) { settings.current.value = v; }
Sample DC::value() { return settings.current.value; }

AudioFrame<1> DC::tick(AudioFrame<0> input) {
  return AudioFrame<1> {settings.current.value};
}

} // namespace Clover::NodeSimplex::Envelope