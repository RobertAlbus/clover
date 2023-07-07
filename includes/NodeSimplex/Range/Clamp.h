#pragma once

#include "Graph.h"

namespace Clover::NodeSimplex::Range {

struct ClampSettings {
  float min;
  float max;
};

template <size_t __arity>
class Clamp : public Graph::StatefulProcessor<__arity, __arity, ClampSettings> {
public:
  Clamp(ClampSettings initialSettings)
      : Graph::StatefulProcessor<__arity, __arity, ClampSettings>(
            initialSettings) {}

  Clamp(float min, float max)
      : Graph::StatefulProcessor<__arity, __arity, ClampSettings>() {
    this->settings.initial.min = min;
    this->settings.initial.max = max;

    this->settings.reset();
  }

  float min() { return this->settings.current.min; }
  float max() { return this->settings.current.max; }

  void min(float minimum) { this->settings.current.min = minimum; }
  void max(float maximum) { this->settings.current.max = maximum; }

private:
  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    Graph::AudioFrame<__arity> f;
    for (int i = 0; i < __arity; i++) {
      ClampSettings &s;
      f[i] = std::clamp(input[i], s.min, s.max);
    }
    return f;
  }
};

} // namespace Clover::NodeSimplex::Range