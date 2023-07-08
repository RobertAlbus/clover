#include <cmath> // lerp

#include "Graph.h"
#include "NodeSimplex/Stereo/Pan.h"

namespace Clover::NodeSimplex::Stereo {

Pan1::Pan1(float p) : Pan<1>(p) {}

Graph::AudioFrame<2> Pan1::tick(Graph::AudioFrame<1> input) {

  Graph::AudioFrame<2> f{input[0] * _coefficientL, input[0] * _coefficientR};
  return f;
}

Pan2::Pan2(float p) : Pan<2>(p) {}

Graph::AudioFrame<2> Pan2::tick(Graph::AudioFrame<2> input) {
  Graph::AudioFrame<2> f{input[0] * _coefficientL, input[1] * _coefficientR};
  return f;
}

} // namespace Clover::NodeSimplex::Stereo