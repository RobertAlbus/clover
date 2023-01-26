#include <functional>
#include <vector>

#include "SampleClock.h"
namespace Clover::Util {

SampleClock::SampleClock() {
  callbacks.reserve(10);
  _currentSample = 0;
}

int SampleClock::currentSample() { return _currentSample; }

void SampleClock::tick() {
  for (int i = 0, end = callbacks.size(); i < end; i++) {
    callbacks[i](_currentSample);
  }
  _currentSample++;
}

void SampleClock::registerTickCallback(ClockCallbackFn fn) { callbacks.emplace_back(fn); }

} // namespace Clover::Util