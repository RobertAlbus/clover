#pragma once

#include <cmath>
#include <algorithm>

#include "graph.h"

namespace Clover::NodeSimplex::Envelope {

struct BasicEnvelopeSettings
{
  size_t startTime;
  size_t targetTime;
  size_t duration;
  float startValue;
  float targetValue;
};

class BasicEnvelope : public StatefulProcessor<0,1, BasicEnvelopeSettings>
{
public:
  BasicEnvelope(BasicEnvelopeSettings initialSettings) : StatefulProcessor(initialSettings)
  {
    set(
      initialSettings.startValue,
      initialSettings.targetValue,
      initialSettings.duration
    );
  }

  BasicEnvelope(float currentValue, float targetValue, size_t durationTime) : StatefulProcessor()
  {
    set(currentValue, targetValue, durationTime);
  }

  void set(float targetValue)
  {
    target(targetValue);
    updateTargetTime();
  }
  void reset(float targetValue)
  {
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

protected:

  void target(float t) {
    settings.current.targetValue = t;
  }

  void current(float currentValue) {
    settings.current.startValue = currentValue;
  }

  void dur(size_t d) {
    settings.current.duration = std::max(d,0UL);
  }

  void updateTargetTime()
  {
    BasicEnvelopeSettings& s = settings.current;
    s.startTime = _currentClockTime;
    s.targetTime = s.startTime + s.duration;
  }

  Frame<1> tick(Frame<0> inputFrame)
  {
    BasicEnvelopeSettings& s = settings.current;

    if (_currentClockTime > s.targetTime) return Frame<1> {s.targetValue};
    double elapsedTime = _currentClockTime - s.startTime;

    return Frame<1>
    {
      std::lerp(s.startValue, s.targetValue, (float)(elapsedTime / s.duration))
    };
  }

};

}