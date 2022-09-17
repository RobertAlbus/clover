#pragma once

#include "../nodeSimplex.h"

namespace Clover::NodeSimplex::Stereo {

auto differenceFn = [](Frame<2> input)
{
    return Frame<2>
    {
        input[0] - input[1],
        input[1] - input[0]
    };
};
class Difference : public StatelessProcessor<2,2,differenceFn> { };

}