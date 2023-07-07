#pragma once

#include "NodeSimplex.h"

namespace Clover::NodeSimplex::Stereo {

class Invert : public Graph::AudioNode<2, 2> {

  Graph::AudioFrame<2> tick(Graph::AudioFrame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo