#include "NodeSimplex/Stereo/Difference.h"

namespace Clover::NodeSimplex::Stereo {

AudioFrame<2> Difference::tick(AudioFrame<2> input) {
  return AudioFrame<2>{input[0] - input[1], input[1] - input[0]};
};

} // namespace Clover::NodeSimplex::Stereo