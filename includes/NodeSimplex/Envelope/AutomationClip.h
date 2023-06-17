#pragma once

#include <cmath>

#include "Algorithm.h"
#include "Graph.h"

class AutomationNode : public AudioNode<0, 1> {
public:
  AutomationNode() : _tensionScale(M_PI), _currentIndex(0), _nextIndex(1) {
    EnvelopeDefinition emptyEnvelope;
    emptyEnvelope.addPoint(0, 0, 0);
    useEnvelope(emptyEnvelope);
  }

  AutomationNode(const EnvelopeDefinition &envelopeDefinition)
      : _tensionScale(M_PI), _currentIndex(0), _nextIndex(1) {
    useEnvelope(envelopeDefinition);
  }

  void useEnvelope(const EnvelopeDefinition &envelopeDefinition) {
    computedEnvelope = envelopeDefinition.compute();
  }

  void tensionScale(float scale) { _tensionScale = scale; }

  float tensionScale() { return _tensionScale; }

private:
  AudioFrame<1> tick(AudioFrame<0>) {
    const EnvelopeComputation::Point &endPoint = computedEnvelope.points.back();

    if (_currentClockTime >= endPoint.start) {
      return AudioFrame<1>{endPoint.value};
    }

    const EnvelopeComputation::Point &currentPoint =
        computedEnvelope.points[_currentIndex];
    const EnvelopeComputation::Point &nextPoint =
        computedEnvelope.points[_nextIndex];

    float elapsedSectionTime = _currentClockTime - currentPoint.start;
    float sectionDuration = nextPoint.start - currentPoint.start;

    float lerpAmount = elapsedSectionTime / sectionDuration;
    float lerpValue =
        std::lerp(currentPoint.value, nextPoint.value, lerpAmount);
    float tensionedValue =
        Calc::tension(lerpValue, nextPoint.tension * _tensionScale);

    bool isNextSectionStarted = _currentClockTime == nextPoint.start;

    _currentIndex++;
    _nextIndex++;

    return AudioFrame<1>{tensionedValue};
  }

  EnvelopeComputation computedEnvelope;
  int _currentIndex;
  int _nextIndex;
  float _tensionScale;
};