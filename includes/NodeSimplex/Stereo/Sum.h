#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Stereo {

class Sum1 : public AudioNode<2, 1> {
  Frame<1> tick(Frame<2> input);
};

class Sum2 : public AudioNode<2, 2> {
  Frame<2> tick(Frame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo