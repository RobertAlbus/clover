#pragma once

// #include "Graph.h"

namespace Clover::NodeSimplex::Range {

struct AvoidSettings {
  float min;
  float max;
  float center;
};

template <size_t __arity>
class Avoid : public Graph::StatefulProcessor<__arity, __arity, AvoidSettings> {
public:
  Avoid(AvoidSettings initialSettings)
      : Graph::StatefulProcessor<__arity, __arity, AvoidSettings>(initialSettings) {}

  Avoid(float minimum, float maximum)
      : Graph::StatefulProcessor<__arity, __arity, AvoidSettings>() {
    this->settings.initial.min = minimum;
    this->settings.initial.max = maximum;
    this->settings.initial.center = findCenter(minimum, maximum);

    this->settings.reset();
  }

  float min() { return this->settings.current.min; }
  float max() { return this->settings.current.max; }

  void min(float minimum) {
    this->settings.current.min = minimum;
    this->settings.current.center =
        findCenter(minimum, this->settings.current.max);
  }

  void max(float maximum) {
    this->settings.current.max = maximum;
    this->settings.current.center =
        findCenter(this->settings.current.min, maximum);
  }

private:
  float findCenter(float minimum, float maximum) {
    return minimum + (maximum - minimum) / 2.;
  }

  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    Graph::AudioFrame<__arity> f;
    for (int i = 0; i < __arity; i++) {
      float sample = input[i];
      AvoidSettings &s = this->settings.current;

      bool isInUpperHalf = sample >= s.center && sample <= s.max;
      bool isInLowerHalf = sample >= s.min && sample < s.center;
      bool canPass = !isInUpperHalf && !isInLowerHalf;

      if (canPass) {
        f[i] = sample;
      } else if (isInUpperHalf) {
        f[i] = s.max;
      } else {
        f[i] = s.min;
      }
    }

    return f;
  }
};

} // namespace Clover::NodeSimplex::Range