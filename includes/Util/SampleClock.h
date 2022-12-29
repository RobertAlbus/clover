#pragma once
#include <functional>
#include <vector>

namespace Clover::Util {

typedef std::function<void(int)> ClockCallbackFn;

class SampleClock {
public:
  SampleClock() {
    callbacks.reserve(10);
    _currentSample = 0;
  }

  int currentSample() { return _currentSample; }

  void tick() {
    for (int i = 0, end = callbacks.size(); i < end; i++) {
      callbacks[i](_currentSample);
    }
    _currentSample++;
  }

  void registerTickCallback(ClockCallbackFn fn) { callbacks.emplace_back(fn); }

private:
  int _currentSample;
  std::vector<ClockCallbackFn> callbacks;
};

} // namespace Clover::Util