#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Stereo {

class Difference : public AudioNode<2, 2> {
  AudioFrame<2> tick(AudioFrame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo