#pragma once

#include <algorithm>

#include "calc.h"
#include "node.h"

using namespace calc;

class Pan1 : public Node<1,2>
{
public:
    Pan1() : Node<1,2>(), _pan(0)
    {
    }

    Pan1(float p) : Node<1,2>(), _pan(0)
    {
        pan(p);
    }

    void pan(float p)
    {
        _pan = (float)std::clamp((double)p, -1., 1.);
        p = fabs(_pan);
        float midGain = dbtol(-4.5);
        float panDown = lerp(midGain, 0., p);
        float panUp   = lerp(midGain, 1., p);

        _coefficientL = _pan < 0 ? panUp   : panDown;
        _coefficientR = _pan < 0 ? panDown : panUp;
    }

    float pan()
    {
        return _pan;
    }

private:
    Frame<2> tick(Frame<1> input)
    {
        
        Frame<2> f {input[0] * _coefficientL, input[0] * _coefficientR};
        return f;
    }

    float _pan;
    float _coefficientL;
    float _coefficientR;
};