#pragma once

#include "Algo/DynamicRange/AsymClip.h"
#include "Graph/AudioNode.h"

namespace Clover::NodeSimplex::DynamicRange {

template <size_t __arity>
class AsymClip : public Graph::AudioNode<__arity, __arity> {
public:
  AsymClip() : AsymClip(-1.f, 1.f) {}

  AsymClip(float minimum, float maximum)
      : Graph::AudioNode<__arity, __arity>(), clip(minimum, maximum) {}

  float min() { return clip.min(); }
  float max() { return clip.max(); }

  void min(float minimum) { clip.min(minimum); }
  void max(float maximum) { clip.max(maximum); }

private:
  Clover::DynamicRange::AsymClip<Sample, __arity> clip;

  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    return Graph::AudioFrame<__arity>{clip.process(input.data)};
  }
};

} // namespace Clover::NodeSimplex::DynamicRange