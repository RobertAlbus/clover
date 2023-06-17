
#include "Graph.h"
#include "NodeSimplex/Stereo/Sum.h"

namespace Clover::NodeSimplex::Stereo {

AudioFrame<1> Sum1::tick(AudioFrame<2> input) {
  return AudioFrame<1>{(input[0] + input[1]) * 0.5f};
};

AudioFrame<2> Sum2::tick(AudioFrame<2> input) {
  return AudioFrame<2>{(input[0] + input[1]) * 0.5f, (input[0] + input[1]) * 0.5f};
};

} // namespace Clover::NodeSimplex::Stereo