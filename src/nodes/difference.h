#pragma once

#include "node.h"

class Difference : public Node<2,2>
{
private:
    Frame<2> tick(Frame<2> input)
    {
        Frame<2> f
        {
            input[0] - input[1],
            input[1] - input[0]
        };

        return f;
    }
};
