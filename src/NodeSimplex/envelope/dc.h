#pragma once

#include "node.h"

namespace Clover::NodeSimplex::Envelope {

template <size_t __arity>
class DC : public Node<1, 1>
{
public:
  DC(Sample value) : Node<__arity, __arity>(), _value(value) {}

  void   value(Sample v) { _value = v; }
  Sample value()         { return _value; }

protected:
  Sample _value;
  Frame<__arity> tick(Frame<__arity> input)
  {
    Frame<__arity> f {};

    for (int i = 0; i < __arity; i++)
    {
      f[i] = _value + input[i];
    }

    return f;
  }
};

}