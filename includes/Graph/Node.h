#pragma once

#include <functional>
#include <stdexcept>
#include <vector>

#include "Frame.h"
#include "FrameHistory.h"
#include "InterchangeArithmeticConcept.h"

namespace Clover::Graph {

template <InterchangeArithmetic OutputType> class INode {
public:
  virtual void metaTick(int currentClockTime) = 0;
  virtual const OutputType &currentFrame() = 0;
};

/// Base class for all nodes of the graph.
template <InterchangeArithmetic InputType, InterchangeArithmetic OutputType>
class Node : public INode<OutputType> {
public:
  float _gain;
  void gain(float gainOut) { _gain = gainOut; }
  float gain() { return _gain; }
  FrameHistory<OutputType> frames;

  Node()
      : _currentClockTime(-1), _gain(1.) {
    inputNodes.reserve(NODE_MAX_INPUT_CAPACITY);
  }

  std::vector<INode<InputType> *> inputNodes;
  int _currentClockTime;

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