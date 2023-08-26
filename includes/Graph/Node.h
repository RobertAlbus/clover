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


#include <functional>
#include <stdexcept>
#include <vector>

#include "AudioFrame.h"
#include "FrameConcept.h"
#include "FrameHistory.h"

namespace Clover::Graph {

template <Frame OutputType> class INode {
public:
  virtual void metaTick(int currentClockTime) = 0;
  virtual const OutputType &currentFrame() = 0;
};

/// Base class for all nodes of the graph.
template <Frame InputType, Frame OutputType>
class Node : public INode<OutputType> {
public:
  int NODE_MAX_INPUT_CAPACITY = 10;
  FrameHistory<OutputType> frames;

  Node() : _currentClockTime(-1), _gain(1.) {
    inputNodes.reserve(NODE_MAX_INPUT_CAPACITY);
  }

  std::vector<INode<InputType> *> inputNodes;
  int _currentClockTime;
  float _gain;

  void gain(float gainOut) { _gain = gainOut; }
  float gain() { return _gain; }

  /// User-defined sample processing method with fallback implementation
  //
  virtual OutputType tick(InputType input) = 0;

  template <typename X> void addInputNode(Node<X, InputType> &inputNode) {
    if (std::find(inputNodes.begin(), inputNodes.end(), &inputNode) !=
        inputNodes.end()) {
      printf("\nAttempted to double connect %p to %p\n", &inputNode, this);
      exit(1);
    }
    inputNodes.emplace_back(&inputNode);
  }

  /// Programmatic tick function that gathers the various Frames
  /// from the input Nodes and passes them to implementation-defined tick()
  void metaTick(int currentClockTime) {
    // Short-circuit any graph cycles
    if (currentClockTime == _currentClockTime)
      return;

    _currentClockTime = currentClockTime;
    tickInputs(currentClockTime);
    tickCallback(currentClockTime);

    accumulationFrame.init();

    for (int i = 0, end = inputNodes.size(); i < end; i++) {
      accumulationFrame += (inputNodes.at(i))->currentFrame();
    }

    processedFrame = tick(accumulationFrame);

    processedFrame *= _gain;
    frames.push(processedFrame);
  }

  const OutputType &currentFrame() { return frames.current; }

private:
  InputType accumulationFrame = {};
  OutputType processedFrame{};
  /// Advance time for all input nodes
  ///
  void tickInputs(int currentClockTime) {
    for (int i = 0, end = inputNodes.size(); i < end; i++) {
      (inputNodes.at(i))->metaTick(currentClockTime);
    }
  }

  /// override this method to add functionality to hook into Node::metaTick(int
  /// currentClockTime)
  virtual void tickCallback(int currentClockTime) {}
};

/// Add left Node to the right Node.inputNodes
///
template <typename X, typename Y, typename Z>
Node<Y, Z> &operator>>(Node<X, Y> &sourceNode, Node<Y, Z> &destinationNode) {
  printf("\n%p   >> %p", &sourceNode, &destinationNode);
  destinationNode.addInputNode(sourceNode);
  return destinationNode;
}

template <typename X, typename Y, typename Z>
Node<Y, Z> &operator>>(Node<X, Y> *sourceNode, Node<Y, Z> &destinationNode) {
  return *(sourceNode) >> destinationNode;
}

template <typename X, typename Y, typename Z>
Node<Y, Z> &operator>>(Node<X, Y> &sourceNode, Node<Y, Z> *destinationNode) {
  return sourceNode >> *(destinationNode);
}

} // namespace Clover::Graph
