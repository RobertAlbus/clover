#pragma once

#include <array>

#include "node.h"
#include "ringBuffer.h"

template <size_t __arity, size_t __bufferSize>
class Delay : public Node<__arity, __arity>
{
public:
  Delay(size_t delayTime = __bufferSize) : Node<__arity, __arity>(), buffer(delayTime)
  {
    setDelayTime(delayTime);
  }

  void setDelayTime(size_t time)
  {
    buffer.setDelayTime(time);
  }

protected:
  RingBuffer<__arity, __bufferSize> buffer;
  Frame<__arity> tick(Frame<__arity> input)
  {
    buffer.write(input);
    Frame<__arity> f {buffer.read()};
    buffer.advance();

    return f;
  }
};