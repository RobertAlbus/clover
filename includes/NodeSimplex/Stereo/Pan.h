#pragma once

#include <cmath> // lerp

#include "Graph.h"
#include "Util/Calc.h"

using namespace Clover::Util;

namespace Clover::NodeSimplex::Stereo {

template <size_t __arityInput> class Pan : public Graph::AudioNode<__arityInput, 2> {
public:
  Pan(float p = 0)
      : Graph::AudioNode<__arityInput, 2>(), _pan(0), midGain(Calc::dbtol(-4.5)) {
    pan(p);
  }

  void pan(float p) {
    _pan = std::clamp<float>(p, -1.f, 1.f);
    p = fabs(_pan);
    float panDown = std::lerp(midGain, 0.f, p);
    float panUp = std::lerp(midGain, 1.f, p);

    _coefficientL = _pan < 0 ? panUp : panDown;
    _coefficientR = _pan < 0 ? panDown : panUp;
  }

  float pan() { return _pan; }

protected:
  float _pan;
  float midGain;
  float _coefficientL;
  float _coefficientR;
};

class Pan1 : public Pan<1> {
public:
  Pan1(float p = 0);

private:
  Graph::AudioFrame<2> tick(Graph::AudioFrame<1> input);
};

class Pan2 : public Pan<2> {
public:
  Pan2(float p = 0);

private:
  Graph::AudioFrame<2> tick(Graph::AudioFrame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo