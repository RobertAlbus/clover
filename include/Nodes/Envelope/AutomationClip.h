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

#include "Algorithm.h"
#include "EnvelopeStruct.h"
#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"

#include "Util/FloatingPointConcept.h"

namespace Clover::Nodes {

template <FloatingPoint Sample = float>
class AutomationNode : public Graph::AudioNode<0, 1, Sample> {
public:
  AutomationNode() : _tensionScale(M_PI), _currentIndex(0), _nextIndex(1) {
    EnvelopeDefinition<Sample> emptyEnvelope;
    emptyEnvelope.addPoint(0, 0, 0);
    useEnvelope(emptyEnvelope);
  }

  AutomationNode(const EnvelopeDefinition<Sample> &envelopeDefinition)
      : _tensionScale(M_PI), _currentIndex(0), _nextIndex(1) {
    useEnvelope(envelopeDefinition);
  }

  void useEnvelope(const EnvelopeDefinition<Sample> &envelopeDefinition) {
    computedEnvelope = envelopeDefinition.compute();
  }

  void tensionScale(Sample scale) { _tensionScale = scale; }

  Sample tensionScale() { return _tensionScale; }

private:
  Graph::AudioFrame<1> tick(Graph::AudioFrame<0>) {
    const typename EnvelopeComputation<Sample>::Point &endPoint = computedEnvelope.points.back();

    if (this->_currentClockTime >= endPoint.start) {
      return Graph::AudioFrame<1>{endPoint.value};
    }

    const typename EnvelopeComputation<Sample>::Point &currentPoint =
        computedEnvelope.points[_currentIndex];
    const typename EnvelopeComputation<Sample>::Point &nextPoint =
        computedEnvelope.points[_nextIndex];

    Sample elapsedSectionTime =
        static_cast<Sample>(this->_currentClockTime - currentPoint.start);
    Sample sectionDuration =
        static_cast<Sample>(nextPoint.start - currentPoint.start);

    Sample lerpAmount = elapsedSectionTime / sectionDuration;
    Sample lerpValue =
        std::lerp(currentPoint.value, nextPoint.value, lerpAmount);
    Sample tensionedValue =
        Algorithm::tension(lerpValue, nextPoint.tension * _tensionScale);

    if (this->_currentClockTime >= nextPoint.start) {
      _currentIndex++;
      _nextIndex++;
    }

    return Graph::AudioFrame<1>{tensionedValue};
  }

  EnvelopeComputation<Sample> computedEnvelope;
  Sample _tensionScale;
  int _currentIndex;
  int _nextIndex;
};

} // namespace Clover::Nodes
