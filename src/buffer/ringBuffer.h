#pragma once

#include <array>
#include <algorithm>


#include "constants.h"
#include "frame.h"


template <size_t __arity, size_t __bufferSize>
class RingBuffer
{
public:
  RingBuffer(size_t delayTime = __bufferSize) : writeHead(__bufferSize-1), buffer {}
  {
    setDelayTime(delayTime);
  }

  void setDelayTime(size_t time)
  {
    time = std::min(time, __bufferSize);
    readHead = (int) ((size_t) writeHead + __bufferSize - (time - 1)) % __bufferSize;
  }

  Frame<__arity> read()
  {
    return buffer[readHead];
  }

  void write(Frame<__arity> f)
  {
    buffer[writeHead] = f;
  }

  void advance()
  {
    readHead  = (readHead  + 1) % __bufferSize;
    writeHead = (writeHead + 1) % __bufferSize;
  }

protected:
  std::array<Frame<__arity>, __bufferSize> buffer;
  int readHead;
  int writeHead;
};