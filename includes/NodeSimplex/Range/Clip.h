#pragma once

#include <algorithm>
#include <functional>

#include "Graph.h"

namespace Clover::NodeSimplex::Range {

struct ClipSettings {
  float threshold;
  std::function<float(float)> clipFunction;
};

template <size_t __arity>
class Clip : public StatefulProcessor<__arity, __arity, ClipSettings> {
public:
  Clip(ClipSettings initialSettings)
      : StatefulProcessor<__arity, __arity, ClipSettings>(initialSettings) {}

  Clip(float threshold) : StatefulProcessor<__arity, __arity, ClipSettings>() {
    this->settings.initial.threshold = threshold;
    this->settings.initial.clipFunction = Clip::clipAboveFunction;

    this->settings.reset();
  }

  float threshold() { return this->settings.current.threshold; }
  void threshold(float threshold) {
    this->settings.current.threshold = threshold;
  }

  void clipAbove() { this->settings.current.clipFunction = clipAboveFunction; }
  void clipBelow() { this->settings.current.clipFunction = clipBelowFunction; }

  std::function<float(float)> clipBelowFunction = [this](Sample s) {
    return std::max(s, this->settings.current.threshold);
  };

  std::function<float(float)> clipAboveFunction = [this](Sample s) {
    return std::min(s, this->settings.current.threshold);
  };

private:
  Frame<__arity> tick(Frame<__arity> input) {
    Frame<__arity> f;
    for (int i = 0; i < __arity; i++) {
      ClipSettings &s;
      f[i] = s.clipFunction(input[i]);
    }
    return f;
  }
};

} // namespace Clover::NodeSimplex::Range