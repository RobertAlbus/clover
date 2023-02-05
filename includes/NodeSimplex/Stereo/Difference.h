#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Stereo {

class Difference : public StatelessProcessorV2<2, 2> {
  Frame<2> processFn(Frame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo