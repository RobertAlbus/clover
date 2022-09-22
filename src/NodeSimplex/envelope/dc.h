#pragma once

#include "node.h"

namespace Clover::NodeSimplex::Envelope {

// consider making this arity-templated if DC is needed on more than one channel at a time.
class DC : public Node<1, 1>
{
public:
  DC(Sample value) : Node<1, 1>(), _value(value) {}

  void value(Sample v)
  {
    _value = v;
  }

protected:
  Sample _value;
  Frame<1> tick(Frame<1> input)
  {
    return Frame<1> {_value + input[0]};
  }
};

}