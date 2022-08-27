#pragma once

#include "node.h"


class DC : public Node<0, 1>
{
public:
  DC(Sample value) : Node<0, 1>(), _value(value) {}

  void value(Sample v)
  {
    _value = v;
  }

protected:
  Sample _value;
  Frame<1> tick(Frame<0> input)
  {
    return Frame<1> {_value};
  }
};