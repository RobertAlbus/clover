#pragma once

#include "Algo/Stereo/Pan.h"
#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"
#include "Util/Calc.h"

namespace Clover::NodeSimplex::Stereo {

class Pan1 : public Graph::AudioNode<1, 2> {
public:
  Pan1(float p = 0.f);

  void pan(float p);
  float pan();

private:
  Graph::AudioFrame<2> tick(Graph::AudioFrame<1> input);
  Clover::Stereo::PanMono<float> pan_;
};

class Pan2 : public Graph::AudioNode<2, 2> {
public:
  Pan2(float p = 0.f);

  void pan(float p);
  float pan();

private:
  Graph::AudioFrame<2> tick(Graph::AudioFrame<2> input);
  Clover::Stereo::PanStereo<float> pan_;
};

} // namespace Clover::NodeSimplex::Stereo