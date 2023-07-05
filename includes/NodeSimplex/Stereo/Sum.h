#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Stereo {

class Sum1 : public Graph::AudioNode<2, 1> {
  Graph::AudioFrame<1> tick(Graph::AudioFrame<2> input);
};

class Sum2 : public Graph::AudioNode<2, 2> {
  Graph::AudioFrame<2> tick(Graph::AudioFrame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo