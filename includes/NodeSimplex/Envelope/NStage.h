#pragma once

#include <vector>

#include "Graph.h"

namespace Clover::NodeSimplex::Envelope {

template <size_t __stages, size_t __arity>
class NStage : public AudioNode<__arity, __arity> {
public:
  NStage() : AudioNode<__arity, __arity>() {}

  void setStage(int stage, float target, int duration) {}
  void keyOn() { _startTime = _currentClockTime; }
  void keyOff() {}

private:
  int _startTime;

  Frame<__arity> tick(Frame<__arity> input) {
    Frame<__arity> f;
    for (int i = 0; i < __arity; i++) {
      f[i] = 0;
    }
    return f;
  }
};

} // namespace Clover::NodeSimplex::Envelope