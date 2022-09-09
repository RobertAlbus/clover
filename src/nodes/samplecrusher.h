#pragma once

#include <algorithm>

#include "calc.h"
#include "node.h"

class Samplecrusher : public Node<1,1>
{
public:
    Samplecrusher(float range = 1) : Node(),
        range(range), amount(0), heldSample(0), samplesToHold(0) {}

    float range;
    float amount;
    Sample heldSample;
    float samplesToHold;

protected:
    Frame<1> tick(Frame<1> inputFrame)
    {
        if (samplesToHold < 1.)
        {
            samplesToHold += 1 + std::max((range * amount), 0.f);
            heldSample = inputFrame[0];
        }
        samplesToHold--;
        return Frame<1> {heldSample};
    }
};
