#pragma once

#include <cmath>

#include "node.h"


class Envelope : public Node<0,1>
{
public:
  Envelope() : Node()
  {
    set(0.,0.,0);
  }

  Envelope(float currentValue, float targetValue, size_t durationTime) : Node()
  {
    printf("\n\n%f - %f - %d\n", currentValue, targetValue, durationTime);
    set(currentValue, targetValue, durationTime);
  }

  void set(float targetValue)
  {
    target(targetValue);
    updateTargetTime();
  }
  void set(float targetValue, size_t durationTime)
  { 
    target(targetValue);
    dur(durationTime);
    updateTargetTime();
  }
  void set(float currentValue, float targetValue, size_t durationTime)
  {
    target(targetValue);
    dur(durationTime);
    current(currentValue);
    updateTargetTime();
  }

  void target(float t) {
    targetValue = t;
  }

  void current(float currentValue) {
    startValue = currentValue;
  }

  void dur(size_t d) {
    duration = d;
  }


protected:
  size_t startTime;
  size_t targetTime;
  size_t duration;
  float startValue;
  float targetValue;

  void updateTargetTime()
  {
    startTime = _currentClockTime;
    targetTime = startTime + duration;
  }

  Frame<1> tick(Frame<0> inputFrame)
  {
    if (_currentClockTime > targetTime) return Frame<1> {targetValue};
    int currentTime = _currentClockTime;
    int elapsedTime = currentTime - startTime;

    float linearScaledTime = (float)elapsedTime / (float)duration;
    return Frame<1>
    {
      std::lerp(startValue, targetValue, linearScaledTime)
    };
  }

};