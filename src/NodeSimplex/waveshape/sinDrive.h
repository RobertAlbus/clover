#pragma once

#include <cmath>

#include "graph/node.h"

using namespace Clover::Graph;

template <size_t __arity>
class SinDrive : public Node<__arity,__arity>
{
public:
  SinDrive() : Node<__arity,__arity>(),
    _shape(0)
  {

  }

  void  shape(float s) { _shape = s;    }
  float shape()        { return _shape; }

private:
  float _shape;
  Frame<__arity> tick(Frame<__arity> input)
  {
    float shape = ((_shape * 0.5) + 0.5) * M_PI;
    Frame<__arity> f {};
    for (int i = 0; i < __arity; i++)
    {
        f[i] = sin(shape * input[i]);
    }

    return f;
  }
};