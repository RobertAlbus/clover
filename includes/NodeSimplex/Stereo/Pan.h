#pragma once

#include <cmath> // lerp

#include "Graph.h"
#include "Util/Calc.h"

using namespace Clover::Util;

namespace Clover::NodeSimplex::Stereo {

template <size_t __arityInput> class Pan : public Node<__arityInput, 2> {
public:
  Pan(float p = 0) : Node<__arityInput, 2>(), _pan(0) { pan(p); }

  void pan(float p) {
    _pan = (float)std::clamp((double)p, -1., 1.);
    p = fabs(_pan);
    float midGain = Calc::dbtol(-4.5);
    float panDown = std::lerp(midGain, 0., p);
    float panUp = std::lerp(midGain, 1., p);

    _coefficientL = _pan < 0 ? panUp : panDown;
    _coefficientR = _pan < 0 ? panDown : panUp;
  }

  float pan() { return _pan; }

protected:
  float _pan;
  float _coefficientL;
  float _coefficientR;
};

class Pan1 : public Pan<1> {
public:
  Pan1(float p = 0);

private:
  Frame<2> tick(Frame<1> input);
};

class Pan2 : public Pan<2> {
public:
  Pan2(float p = 0);

private:
  Frame<2> tick(Frame<2> input);
};

} // namespace Clover::NodeSimplex::Stereo