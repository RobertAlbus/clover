#pragma once

#include <algorithm>

#include "Graph/Frame.h"
#include "InterchangeConcept.h"

namespace Clover::Graph {

template <Interchange T> class FrameHistory {
public:
  FrameHistory() : _hasNext(false) {}

  void push(T frame) {
    if (!_hasNext)
      _next = frame;
    _hasNext = false;
    // todo: benchmark the current implementation with std::move compared to
    // previous implementation with std::swap
    // this is probably way better
    last = std::move(current);
    current = std::move(_next);
  }

  void next(T frame) {
    _hasNext = true;
    _next = frame;
  }

  T current = {};
  T last = {};

private:
  T _next = {};
  bool _hasNext;
};

} // namespace Clover::Graph