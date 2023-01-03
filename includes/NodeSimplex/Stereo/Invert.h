#pragma once

#include "NodeSimplex.h"

namespace Clover::NodeSimplex::Stereo {

constexpr auto invertFn = [](Frame<2> input) {
  return Frame<2>{input[1], input[0]};
};
class Invert : public StatelessProcessor<2, 2, invertFn> {};

} // namespace Clover::NodeSimplex::Stereo