#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Stereo {

class Sum1 : public StatelessProcessorV2<2, 1> {
  Frame<1> processFn(Frame<2> input);
};

class Sum2 : public StatelessProcessorV2<2, 2> {
  Frame<2> processFn(Frame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo