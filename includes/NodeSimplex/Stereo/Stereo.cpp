#include <cmath>

#include "Graph.h"
#include "NodeSimplex/Stereo/Stereo.h"

#include "NodeSimplex/Stereo/Difference.h"
#include "NodeSimplex/Stereo/Pan.h"
#include "NodeSimplex/Stereo/Sum.h"

using namespace Clover::Util;
namespace Clover::NodeSimplex::Stereo {

StereoImage::StereoImage(float s, float p) : Subgraph(), pan2(p) {
  connectNodes();
  spread(s);
}

void StereoImage::connectNodes() {
  spring >> mid >> pan2;
  spring >> side >> pan2;
  pan2 >> blackHole;
}

void StereoImage::spread(float s) {
  _spreadReadOnly = s;

  s = fabs(_spreadReadOnly);
  float neutralGain = Calc::dbtol(-4.5);
  float gainUp = std::lerp(neutralGain, 0., s);
  float gainDown = std::lerp(neutralGain, 1., s);

  mid.gain(_spreadReadOnly < 0 ? gainUp : gainDown);
  side.gain(_spreadReadOnly < 0 ? gainDown : gainUp);
}

float StereoImage::spread() { return _spreadReadOnly; }

void StereoImage::pan(float p) { pan2.pan(p); }

Frame<2> StereoImage::tick(Frame<2> input) {
  spring.frames.next(input);
  return blackHole.frames.current;
}

}