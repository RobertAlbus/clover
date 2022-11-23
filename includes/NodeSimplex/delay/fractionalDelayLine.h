#pragma once

#include <algorithm>
#include <vector>
#include <math.h>

#include "graph.h"

namespace Clover::NodeSimplex::Delay {

struct FractionalDelayLineSettings
{
  FractionalDelayLineSettings() : FractionalDelayLineSettings(0) {}
  FractionalDelayLineSettings(float delayTime) : delayTime(delayTime) {}
  float delayTime;
};

template <size_t __arity, size_t __bufferSize>
class FractionalDelayLine : public StatefulProcessor<__arity, __arity, FractionalDelayLineSettings>
{
public:
  FractionalDelayLine(FractionalDelayLineSettings initialSettings)
  : StatefulProcessor<__arity, __arity, FractionalDelayLineSettings>(initialSettings),
    buffer{}, writeHead(0), readHead(0)
  {
    buffer.reserve(__bufferSize);
    setDelayTime(initialSettings.delayTime);
  }

  void setDelayTime(float time)
  {
    this->settings.current.delayTime = time;

    float bufferSize = (float) __bufferSize;
    time = std::clamp(time, 0.f, bufferSize-1.f);
    
    readHead = fmod(
      ((float) writeHead - time + bufferSize),
      bufferSize
    );
  }

protected:
  std::vector<Frame<__arity>> buffer;
  float readHead;
  int writeHead;
  Frame<__arity> tick(Frame<__arity> input)
  {
    // push new data to buffer
    buffer[writeHead] = input;
    
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

    // advance the buffer
    readHead  = fmod(readHead + 1.f, (float) __bufferSize);
    writeHead = (writeHead + 1) % __bufferSize;

    return f;
  }
};

}