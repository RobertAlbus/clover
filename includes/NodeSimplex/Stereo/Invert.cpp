#include "NodeSimplex/Stereo/Invert.h"

namespace Clover::NodeSimplex::Stereo {

Frame<2> Invert::processFn(Frame<2> input) { return Frame<2>{input[1], input[0]}; };

} // namespace Clover::NodeSimplex::Stereo