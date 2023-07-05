
#include "Graph.h"
#include "NodeSimplex/Stereo/Sum.h"

namespace Clover::NodeSimplex::Stereo {

Graph::AudioFrame<1> Sum1::tick(Graph::AudioFrame<2> input) {
  return Graph::AudioFrame<1>{(input[0] + input[1]) * 0.5f};
};

Graph::AudioFrame<2> Sum2::tick(Graph::AudioFrame<2> input) {
  return Graph::AudioFrame<2>{(input[0] + input[1]) * 0.5f, (input[0] + input[1]) * 0.5f};
};

} // namespace Clover::NodeSimplex::Stereo