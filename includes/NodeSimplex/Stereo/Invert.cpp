#include "NodeSimplex/Stereo/Invert.h"

namespace Clover::NodeSimplex::Stereo {

Graph::AudioFrame<2> Invert::tick(Graph::AudioFrame<2> input) { return Graph::AudioFrame<2>{input[1], input[0]}; };

} // namespace Clover::NodeSimplex::Stereo