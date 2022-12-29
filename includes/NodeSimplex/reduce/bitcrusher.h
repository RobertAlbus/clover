#pragma once

#include <limits.h>
#include <math.h>

#include "util/calc.h"
#include "Graph.h"

using namespace Clover::Util;

namespace Clover::NodeSimplex::Reduce {

class BitCrusher : public Node<1,1>
{
public:
    BitCrusher(float bits = std::numeric_limits<float>::max()) : Node(), bits(bits) {}

    // generally undetectable at about 600-800
    // I should probably convert this to an "amount" control
    // I can fade out the crush signal in the highest 10%
    // I can constrain the bits value to sensible values
    
    // I might need to do something more extreme for the high-effect
    // position to actually sound decent. the crush is anemic.
    float bits;

protected:
    Frame<1> tick(Frame<1> inputFrame)
    {
        float sign = Calc::sign(inputFrame[0]);
        float abs = fabs(inputFrame[0]);
        abs = floor(abs * bits) / bits;
        return Frame<1> {abs * sign};
    }
};

}