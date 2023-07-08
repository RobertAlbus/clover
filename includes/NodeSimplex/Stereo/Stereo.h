#pragma once

#include "Graph.h"
#include "NodeSimplex/Stereo/Difference.h"
#include "NodeSimplex/Stereo/Pan.h"
#include "NodeSimplex/Stereo/Sum.h"

namespace Clover::NodeSimplex::Stereo {

class StereoImage : public Subgraph<2, 2> {
public:
  StereoImage(float s = 0, float p = 0);

  void connectNodes();

  void spread(float s);

  float spread();

  void pan(float p);

protected:
  Sum2 mid;
  Difference side;
  Pan2 pan2;
  Clover::IO::RootNode<2> spring;

  float _spreadReadOnly;

  Graph::AudioFrame<2> tick(Graph::AudioFrame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo
