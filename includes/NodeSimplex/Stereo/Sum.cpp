
#include "Graph.h"
#include "NodeSimplex/Stereo/Sum.h"

namespace Clover::NodeSimplex::Stereo {

Graph::AudioFrame<1> Sum1::tick(Graph::AudioFrame<2> input) {
  return Graph::AudioFrame<1>{sum.process(input.data)};
};

Graph::AudioFrame<2> Sum2::tick(Graph::AudioFrame<2> input) {
  return Graph::AudioFrame<2>{sum.process(input.data)};
};

} // namespace Clover::NodeSimplex::Stereo