#pragma once

#include "node.h"

using namespace Clover::Graph;

namespace Clover::NodeSimplex::Envelope {

template <size_t __arity>
class Impulse : public Node<__arity,__arity>
{
public:
  Impulse() : Node<__arity,__arity>() _value(0) {}

  void trigger(float value) { _value = value; }
  void trigger()            { keyOn(1);       }

protected:
  float _value;
  Frame<__arity> tick(Frame<__arity> input)
  {
    Frame<__arity> f;
    for (int i = 0; i < __arity; i++)
    {
      f[i] = _value;
    }
    _value = 0;
    return f;
  }
};

}