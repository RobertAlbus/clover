#pragma once

#include <algorithm>

#include "Graph/Frame.h"

namespace Clover::Graph {

template <size_t __arity> class FrameHistory {
public:
  FrameHistory() : _hasNext(false) {}

  void push(Frame<__arity> frame) {
    if (!_hasNext)
      _next = frame;
    _hasNext = false;
    // todo: benchmark the current implementation with std::move compared to
    // previous implementation with std::swap
    // this is probably way better
    last = std::move(current);
    current = std::move(_next);
  }

  void next(Frame<__arity> frame) {
    _hasNext = true;
    _next = frame;
  }

  Frame<__arity> current = {};
  Frame<__arity> last = {};

private:
  Frame<__arity> _next = {};
  bool _hasNext;
};

} // namespace Clover::Graph