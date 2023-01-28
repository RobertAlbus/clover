#include <cmath> // lerp

#include "Graph.h"
#include "Util/Calc.h"
#include "NodeSimplex/Stereo/Pan.h"

using namespace Clover::Util;

namespace Clover::NodeSimplex::Stereo {

Pan1::Pan1(float p) : Pan<1>(p) {}

Frame<2> Pan1::tick(Frame<1> input) {

  Frame<2> f{input[0] * _coefficientL, input[0] * _coefficientR};
  return f;
}

Pan2::Pan2(float p) : Pan<2>(p) {}

Frame<2> Pan2::tick(Frame<2> input) {
  Frame<2> f{input[0] * _coefficientL, input[1] * _coefficientR};
  return f;
}

} // namespace Clover::NodeSimplex::Stereo