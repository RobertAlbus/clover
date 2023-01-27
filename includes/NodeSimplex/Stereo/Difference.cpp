#include "NodeSimplex/Stereo/Difference.h"

namespace Clover::NodeSimplex::Stereo {

Frame<2> Difference::processFn(Frame<2> input) {
  return Frame<2>{input[0] - input[1], input[1] - input[0]};
};

} // namespace Clover::NodeSimplex::Stereo