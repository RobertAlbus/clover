#include "NodeSimplex/Stereo/Invert.h"

namespace Clover::NodeSimplex::Stereo {

AudioFrame<2> Invert::tick(AudioFrame<2> input) { return AudioFrame<2>{input[1], input[0]}; };

} // namespace Clover::NodeSimplex::Stereo