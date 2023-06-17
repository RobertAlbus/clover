#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Stereo {

class Sum1 : public AudioNode<2, 1> {
  AudioFrame<1> tick(AudioFrame<2> input);
};

class Sum2 : public AudioNode<2, 2> {
  AudioFrame<2> tick(AudioFrame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo