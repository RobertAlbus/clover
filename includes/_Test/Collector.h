#pragma once

#include "Graph.h"

using namespace Clover::Graph;
using namespace Clover::Util;

namespace Clover::_Test {

template <size_t __arity> class Collector : public Node<__arity, __arity> {
public:
  Collector(int preAllocateFrames) : Node<__arity, __arity>() {
    frames.reserve(preAllocateFrames);
  }

  std::vector<Frame<__arity>> frames;

private:
  Frame<__arity> tick(Frame<__arity> input) {
    frames.emplace_back(input);

    return input;
  }
};

} // namespace Clover::_Test