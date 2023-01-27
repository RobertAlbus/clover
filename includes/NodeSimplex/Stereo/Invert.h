#pragma once

#include "NodeSimplex.h"

namespace Clover::NodeSimplex::Stereo {

class Invert : public StatelessProcessorV2<2, 2> {
  Frame<2> processFn(Frame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo