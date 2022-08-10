#pragma once

#include <cmath>

#include "node.h"


class Envelope : public Node<0,1>
{
public:
  Envelope() : Node()
  {
    set(0,0,0);
  }

  Envelope(float currentValue, float targetValue, int durationTime) : Node()
  {
    set(currentValue, targetValue, durationTime);
  }

  void set(float targetValue)
  {
    target(targetValue);
    startTime = lastComputedClockTime;
    targetTime = startTime + duration;
  }
  void set(float targetValue, int durationTime)
  { 
    set(targetValue);
    dur(durationTime);
  }
  void set(float currentValue, float targetValue, int durationTime)
  {
    set(targetValue, durationTime);
    current(currentValue);
  }

  void target(float t) {
    targetValue = t;
  }

  void current(float currentValue) {
    startValue = currentValue;
  }

  void dur(int d) {
    duration = d;
  }


protected:
  int startTime;
  int targetTime;
  int duration;
  float startValue;
  float targetValue;

  Frame<1> tick(Frame<0> inputFrame)
  {
    int currentTime = lastComputedClockTime;
    int elapsedTime = currentTime - startTime;

    float linearScaledTime = (float)elapsedTime / (float)duration;
    Frame<1> f = {
      std::lerp(startValue, targetValue, linearScaledTime)
    };
    return f;
  }

};