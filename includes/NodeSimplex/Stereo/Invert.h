#pragma once

#include "NodeSimplex.h"

namespace Clover::NodeSimplex::Stereo {

class Invert : public AudioNode<2, 2> {

  Frame<2> tick(Frame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo