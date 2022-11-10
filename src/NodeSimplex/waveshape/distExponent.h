#pragma once

#include <cmath>

#include "graph/node.h"

using namespace Clover::Graph;
using namespace Clover::Util;

namespace Clover::NodeSimplex::Waveshape 
{

template <size_t __arity>
class DistExponent : public Node<__arity,__arity>
{
public:
  DistExponent() : Node<__arity,__arity>(),
    _exponent(1)
  {

  }

  void  exponent(float e) { _exponent = e;    }
  float exponent()        { return _exponent; }

private:
  float _exponent;
  Frame<__arity> tick(Frame<__arity> input)
  {
    Frame<__arity> f {};
    for (int i = 0; i < __arity; i++)
    {
        float sign = Calc::sign(input[i]);
        float abs = fabs(input[i]);
        f[i] = pow(abs, 1.- _exponent) * sign;
        printf(" ||||      %f | %f | %f | %f | %f | %f\n",
          Node<__arity,__arity>::gainIn(), input[i], sign, abs, _exponent, f[i]
        );
    }

    return f;
  }
};

}
