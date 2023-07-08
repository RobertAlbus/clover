#include "NodeSimplex/Stereo/Difference.h"

namespace Clover::NodeSimplex::Stereo {

Graph::AudioFrame<2> Difference::tick(Graph::AudioFrame<2> input) {
  return Graph::AudioFrame<2>{difference.process(input.data)};
};

} // namespace Clover::NodeSimplex::Stereo