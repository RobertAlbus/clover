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

    // Pan1(float p) : Node<1,2>(), _pan(0)
    // {
    // }

    void pan(float p)
    {
        // _pan = (float)std::clamp((double)p, -1., 1.);
    }

    float pan()
    {
        return _pan;
    }

private:
    Frame<2> tick(Frame<1> input)
    {
        // Frame<2> f {input[0] * ltodb(-4.5), input[0] * ltodb(-4.5)};
        Frame<2> f {input[0], input[0]};

        return f;
    }

    float _pan;
};