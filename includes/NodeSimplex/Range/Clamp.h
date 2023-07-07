#pragma once

#include "Graph/AudioNode.h"

namespace Clover::NodeSimplex::Range {

template <size_t __arity>
class Clamp : public Graph::AudioNode<__arity, __arity> {
public:
  Clamp() : Clamp(-1.f, 1.f) {}

  Clamp(float minimum, float maximum)
      : Graph::AudioNode<__arity, __arity>(), min_(minimum), max_(maximum) {}

  float min() { return min_; }
  float max() { return max_; }

  void min(float minimum) { min_ = minimum; }
  void max(float maximum) { max_ = maximum; }

private:
  float min_;
  float max_;

  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    Graph::AudioFrame<__arity> f;
    for (int i = 0; i < __arity; i++) {
      f[i] = std::clamp(input[i], min_, max_);
    }
    return f;
  }
};

} // namespace Clover::NodeSimplex::Range