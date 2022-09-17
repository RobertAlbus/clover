#pragma once

#include "node.h"

auto sum1Fn = [](Frame<2> input)
{
  return Frame<1> {(input[0] + input[1]) * 0.5f };
};
class Sum1 : public StatelessProcessor<2,1,sum1Fn> { };

auto sum2Fn = [](Frame<2> input)
{
  return Frame<2>
  {
    (input[0] + input[1]) * 0.5f,
    (input[0] + input[1]) * 0.5f
  };
};
class Sum2 : public StatelessProcessor<2,2,sum2Fn> { };