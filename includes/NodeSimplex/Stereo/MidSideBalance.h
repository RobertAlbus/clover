#pragma once

#include "Algo/Stereo/MidSideBalance.h"

#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"

namespace Clover::NodeSimplex::Stereo {

class MidSideBalance : public Graph::AudioNode<2, 2> {
public:
  MidSideBalance(float s = 0);

  float spread();
  void spread(float s);

protected:
  Graph::AudioFrame<2> tick(Graph::AudioFrame<2> input);
  Clover::Stereo::MidSideBalance<float> msBalance;
};

} // namespace Clover::NodeSimplex::Stereo
