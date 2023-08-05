#pragma once

#include "Algo/Stereo/Sum.h"
#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"

namespace Clover::NodeSimplex::Stereo {

class Sum1 : public Graph::AudioNode<2, 1> {
  Graph::AudioFrame<1> tick(Graph::AudioFrame<2> input);
  Clover::Stereo::SumMono<Sample> sum;
};

class Sum2 : public Graph::AudioNode<2, 2> {
  Graph::AudioFrame<2> tick(Graph::AudioFrame<2> input);
  Clover::Stereo::SumStereo<Sample> sum;
};

} // namespace Clover::NodeSimplex::Stereo