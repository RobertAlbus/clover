#include <cmath> // lerp

#include "Graph.h"
#include "NodeSimplex/Stereo/Pan.h"

namespace Clover::NodeSimplex::Stereo {

Pan1::Pan1(float p) : Graph::AudioNode<1,2>(), pan_(p) {}

Graph::AudioFrame<2> Pan1::tick(Graph::AudioFrame<1> input) {

  return Graph::AudioFrame<2>{pan_.process(input[0])};
}

void Pan1::pan(float p) { pan_.pan(p);   }
float Pan1::pan() { return pan_.pan(); }

Pan2::Pan2(float p) : Graph::AudioNode<2,2>(), pan_(p) {}

Graph::AudioFrame<2> Pan2::tick(Graph::AudioFrame<2> input) {
  return Graph::AudioFrame<2>{pan_.process(input.data)};
}

void Pan2::pan(float p) { pan_.pan(p);   }
float Pan2::pan() { return pan_.pan(); }

} // namespace Clover::NodeSimplex::Stereo