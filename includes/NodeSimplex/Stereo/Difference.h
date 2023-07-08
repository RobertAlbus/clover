#pragma once

#include "Algo/Stereo/Difference.h"
#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"

namespace Clover::NodeSimplex::Stereo {

class Difference : public Graph::AudioNode<2, 2> {
  Graph::AudioFrame<2> tick(Graph::AudioFrame<2> input);
  Clover::Stereo::Difference<Sample> difference;
};

} // namespace Clover::NodeSimplex::Stereo