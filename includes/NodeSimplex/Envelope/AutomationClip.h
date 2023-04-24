#pragma once

#include <cmath>
#include "Graph.h"

#include "Algorithm.h"

class AutomationNode : public Node<0, 1> {
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
  Frame<1> tick(Frame<0>) {
    const EnvelopeComputation::Point &endPoint = computedEnvelope.points.back();

    if (_currentClockTime >= endPoint.start) {
      return Frame<1>{endPoint.value};
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

    return Frame<1>{tensionedValue};
  }

  EnvelopeComputation computedEnvelope;
  int _currentIndex;
  int _nextIndex;
  float _tensionScale;
};