#pragma once

#include <algorithm>
#include <vector>
#include <math.h>

#include "node.h"

namespace Clover::NodeSimplex::Delay {

template <size_t __arity, size_t __bufferSize>
class FractionalDelayLine : public Node<__arity, __arity>
{
public:
  FractionalDelayLine(float delayTime) : 
    Node<__arity, __arity>(),
    buffer{}, writeHead(0), readHead(0)
  {
    buffer.reserve(__bufferSize);
    setDelayTime(delayTime);
  }

  void setDelayTime(float time)
  {
    float bufferSize = (float) __bufferSize;
    time = std::clamp(time, 0.f, bufferSize-1.f);
    
    readHead = (float) writeHead - time;
    
    // ensure value wraps around the buffer size
    readHead = fmod(
      (readHead + bufferSize),
      bufferSize
    );
  }

protected:
  std::vector<Frame<__arity>> buffer;
  float readHead;
  int writeHead;
  Frame<__arity> tick(Frame<__arity> input)
  {

    // linear interpolate between current and  next frame
    int previousReadHead = (int) readHead;
    int nextReadHead = (previousReadHead + 1) %  __bufferSize;
    float lerpAmount = readHead - (float) previousReadHead;

    Frame<__arity> f {};
    for (int i = 0; i <__arity; i++)
    {
      f[i] = std::lerp(
        buffer[previousReadHead][i],
        buffer[nextReadHead][i],
        lerpAmount
        );
    }
    
    // push new data to buffer
    buffer[writeHead] = input;

    // advance the buffer
    readHead  = fmod(readHead + 1.f, (float) __bufferSize);
    writeHead = (writeHead + 1) % __bufferSize;

    return f;
  }
};

}