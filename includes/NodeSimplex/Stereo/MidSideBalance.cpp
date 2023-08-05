#include <cmath>

#include "NodeSimplex/Stereo/MidSideBalance.h"

#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"

namespace Clover::NodeSimplex::Stereo {

MidSideBalance::MidSideBalance(float s) : AudioNode<2,2>(), msBalance(s) {}

void MidSideBalance::spread(float s) { msBalance.spread(s); }

float MidSideBalance::spread() { return msBalance.spread(); }

Graph::AudioFrame<2> MidSideBalance::tick(Graph::AudioFrame<2> input) {
  return msBalance.process(input.data);
}

}