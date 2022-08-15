#pragma once

#include <array>

#include "node.h"
#include "ringBuffer.h"

template <size_t __arity, size_t __bufferSize>
class Delay : public Node<__arity, __arity>
{
public:
  Delay() : Node<__arity, __arity>() {}

  RingBuffer<__arity, __bufferSize> buffer;
protected:

  Frame<__arity> tick(Frame<__arity> input)
  {
    buffer.write(input);
    Frame<__arity> f {buffer.read()};
    buffer.advance();

    return f;
  }
};