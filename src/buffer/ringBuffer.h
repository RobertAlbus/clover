#pragma once

#include <array>

#include "constants.h"
#include "frame.h"


template <size_t __arity, size_t __bufferSize>
class RingBuffer
{
public:
  RingBuffer() : readHead(0), writeHead(__bufferSize-1), buffer {}
  {

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