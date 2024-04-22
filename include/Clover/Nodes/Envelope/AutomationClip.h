#pragma once

/*
 * /////////
 * // Clover
 *
 * Audio processing algorithms and DAG with feedback loops that do not break
 * acyclicity.
 *
 * Copyright (C) 2023 Rob W. Albus
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <cmath>

#include "Clover/Algorithm/Math.h"
#include "EnvelopeStruct.h"
#include "Clover/Graph/AudioFrame.h"
#include "Clover/Graph/AudioNode.h"

namespace Clover::Nodes {

class AutomationNode : public Graph::AudioNode<0, 1> {
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
  Graph::AudioFrame<1> tick(Graph::AudioFrame<0>) {
    const EnvelopeComputation::Point &endPoint = computedEnvelope.points.back();

    if (this->_currentClockTime >= endPoint.start) {
      return Graph::AudioFrame<1>{endPoint.value};
    }

    const EnvelopeComputation::Point &currentPoint =
        computedEnvelope.points[_currentIndex];
    const EnvelopeComputation::Point &nextPoint =
        computedEnvelope.points[_nextIndex];

    float elapsedSectionTime =
        static_cast<float>(_currentClockTime - currentPoint.start);
    float sectionDuration =
        static_cast<float>(nextPoint.start - currentPoint.start);

    float lerpAmount = elapsedSectionTime / sectionDuration;
    float lerpValue =
        std::lerp(currentPoint.value, nextPoint.value, lerpAmount);
    float tensionedValue =
        Algorithm::tension(lerpValue, nextPoint.tension * _tensionScale);

    if (_currentClockTime >= nextPoint.start) {
      _currentIndex++;
      _nextIndex++;
    }

    return Graph::AudioFrame<1>{tensionedValue};
  }

  EnvelopeComputation computedEnvelope;
  float _tensionScale;
  int _currentIndex;
  int _nextIndex;
};

} // namespace Clover::Nodes
