#pragma once

#include "NodeSimplex.h"

namespace Clover::NodeSimplex::Stereo {

class Invert : public AudioNode<2, 2> {

  AudioFrame<2> tick(AudioFrame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo