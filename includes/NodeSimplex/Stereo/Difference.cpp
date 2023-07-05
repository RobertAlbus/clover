#include "NodeSimplex/Stereo/Difference.h"

namespace Clover::NodeSimplex::Stereo {

Graph::AudioFrame<2> Difference::tick(Graph::AudioFrame<2> input) {
  return Graph::AudioFrame<2>{input[0] - input[1], input[1] - input[0]};
};

} // namespace Clover::NodeSimplex::Stereo