#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Stereo {

class Difference : public Graph::AudioNode<2, 2> {
  Graph::AudioFrame<2> tick(Graph::AudioFrame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo