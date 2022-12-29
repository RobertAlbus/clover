#pragma once

#include <cmath>

#include "Difference.h"
#include "Graph.h"
#include "Pan.h"
#include "Sum.h"

using namespace Clover::NodeSimplex::Stereo;
using namespace Clover::Util;

class StereoImage : public Subgraph<2, 2> {
public:
  StereoImage() : Subgraph(), pan2(0) {
    connectNodes();
    spread(0);
  }

  StereoImage(float s, float p) : Subgraph(), pan2(p) {
    connectNodes();
    spread(s);
  }

  void connectNodes() {
    spring >> mid >> pan2;
    spring >> side >> pan2;
    pan2 >> blackHole;
  }

  void spread(float s) {
    _spreadReadOnly = s;

    s = fabs(_spreadReadOnly);
    float neutralGain = Calc::dbtol(-4.5);
    float gainUp = lerp(neutralGain, 0., s);
    float gainDown = lerp(neutralGain, 1., s);

    mid.gain(_spreadReadOnly < 0 ? gainUp : gainDown);
    side.gain(_spreadReadOnly < 0 ? gainDown : gainUp);
  }

  float spread() { return _spreadReadOnly; }

  void pan(float p) { pan2.pan(p); }

protected:
  Sum2 mid;
  Difference side;
  Pan2 pan2;
  Clover::IO::RootNode<2> spring;

  float _spreadReadOnly;

  Frame<2> tick(Frame<2> input) override {
    spring.frames.next(input);
    return blackHole.frames.current;
  }
};