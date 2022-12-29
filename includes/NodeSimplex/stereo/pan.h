#pragma once

#include <algorithm>

#include "util/calc.h"
#include "Graph.h"

using namespace Clover::Util;

namespace Clover::NodeSimplex::Stereo {

template <size_t __arityInput>
class Pan : public Node<__arityInput,2>
{
public:
    Pan() : Node<__arityInput,2>()
    {
        pan(0);
    }

    Pan(float p) : Node<__arityInput,2>(), _pan(0)
    {
        pan(p);
    }

    void pan(float p)
    {
        _pan = (float)std::clamp((double)p, -1., 1.);
        p = fabs(_pan);
        float midGain = Calc::dbtol(-4.5);
        float panDown = lerp(midGain, 0., p);
        float panUp   = lerp(midGain, 1., p);

        _coefficientL = _pan < 0 ? panUp   : panDown;
        _coefficientR = _pan < 0 ? panDown : panUp;
    }

    float pan()
    {
        return _pan;
    }

protected:
    float _pan;
    float _coefficientL;
    float _coefficientR;
};

class Pan1 : public Pan<1>
{
public:
    Pan1() : Pan() { }
    Pan1(float p) : Pan(p) { }

private:
    Frame<2> tick(Frame<1> input)
    {
        
        Frame<2> f {input[0] * _coefficientL, input[0] * _coefficientR};
        return f;
    }

};

class Pan2 : public Pan<2>
{
public:
    Pan2() : Pan() { }
    Pan2(float p) : Pan(p) { }

private:
    Frame<2> tick(Frame<2> input)
    {
        Frame<2> f {input[0] * _coefficientL, input[1] * _coefficientR};
        return f;
    }

};

}