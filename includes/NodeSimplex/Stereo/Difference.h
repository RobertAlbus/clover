#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Stereo {

class Difference : public Node<2, 2> {
  Frame<2> tick(Frame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo