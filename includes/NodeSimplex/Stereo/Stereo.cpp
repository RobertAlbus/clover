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
  if (_spreadReadOnly == s) return;
  _spreadReadOnly = s;

  s = fabs(_spreadReadOnly);
  float neutralGain = Calc::dbtol(-4.5f);
  float gainUp = std::lerp(neutralGain, 0.f, s);
  float gainDown = std::lerp(neutralGain, 1.f, s);

  mid.gain(_spreadReadOnly < 0 ? gainUp : gainDown);
  side.gain(_spreadReadOnly < 0 ? gainDown : gainUp);
}

float StereoImage::spread() { return _spreadReadOnly; }

void StereoImage::pan(float p) { pan2.pan(p); }

AudioFrame<2> StereoImage::tick(AudioFrame<2> input) {
  spring.frames.next(input);
  return blackHole.frames.current;
}

}