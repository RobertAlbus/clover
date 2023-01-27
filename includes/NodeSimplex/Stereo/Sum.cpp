
#include "Graph.h"
#include "NodeSimplex/Stereo/Sum.h"

namespace Clover::NodeSimplex::Stereo {

Frame<1> Sum1::processFn(Frame<2> input) {
  return Frame<1>{(input[0] + input[1]) * 0.5f};
};

Frame<2> Sum2::processFn(Frame<2> input) {
  return Frame<2>{(input[0] + input[1]) * 0.5f, (input[0] + input[1]) * 0.5f};
};

} // namespace Clover::NodeSimplex::Stereo