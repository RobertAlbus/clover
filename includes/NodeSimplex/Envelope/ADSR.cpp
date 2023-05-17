#include <algorithm>
#include <cmath>

#include "Graph.h"
#include "NodeSimplex/Envelope/BasicEnvelope.h"
#include "NodeSimplex/Envelope/ADSR.h"

namespace Clover::NodeSimplex::Envelope {

AdsrSettings::AdsrSettings(size_t a, size_t d, float s, size_t r)
    : attack(std::max((size_t)1, a)), decay(std::max((size_t)1, d)),
      sustain(s), release(std::max((size_t)1, r)), startTime(0),
      keyOn(false) {}

Adsr::Adsr(size_t a, size_t d, float s, size_t r)
    : StatefulSubgraph(), envelope(0, 0, 0) {
  this->settings.initial.attack = a;
  this->settings.initial.decay = d;
  this->settings.initial.sustain = s;
  this->settings.initial.release = r;

  settings.reset();
  connectNodes();
}

Adsr::Adsr(AdsrSettings initialSettings)
    : StatefulSubgraph(initialSettings), envelope(0, 0, 0) {
  settings.reset();
  connectNodes();
}

void Adsr::set(size_t a, size_t d, float s, size_t r) {
  attack(a);
  decay(d);
  sustain(s);
  release(r);
}

size_t Adsr::attack() { return settings.current.attack; }
void Adsr::attack(size_t a) { settings.current.attack = std::max((size_t)1, a); }

size_t Adsr::decay() { return settings.current.decay; }
void Adsr::decay(size_t d) { settings.current.decay = std::max((size_t)1, d); }

float Adsr::sustain() { return settings.current.sustain; }
void Adsr::sustain(float s) { settings.current.sustain = s; }

size_t Adsr::release() { return settings.current.release; }
void Adsr::release(size_t r) { settings.current.release = std::max((size_t)1, r); }

void Adsr::keyOn() {
  AdsrSettings &s = settings.current;
  envelope.set(0.f, 1.f, s.attack);

  settings.current.keyOn = true;
  settings.current.startTime = _currentClockTime;
}

void Adsr::keyOff() {
  AdsrSettings &s = settings.current;
  float currentValue = frames.current[0] / _gain;

  float percentOfSustain = currentValue / s.sustain;

  // todo: add a test for early release scenario
  float releaseDuration = s.release * std::min(percentOfSustain, 1.f);

  envelope.set(currentValue, 0., releaseDuration);

  settings.current.keyOn = false;
  settings.current.startTime = _currentClockTime;
}

void Adsr::triggerOn() {
  keyOn();
}
void Adsr::triggerOff() {
  keyOff();
}

Frame<1> Adsr::tick(Frame<0> input) {
  AdsrSettings &s = settings.current;

  // should compute these times and store them in settings
  bool isStartOfDecay =
      s.keyOn && _currentClockTime == s.startTime + s.attack;
  bool isStartOfSustain =
      s.keyOn && _currentClockTime == s.startTime + s.attack + s.decay;

  if (isStartOfSustain) {
    envelope.set(s.sustain, s.sustain, 0);
  } else if (isStartOfDecay) {
    envelope.set(1., s.sustain, s.decay);
  }

  return blackHole.frames.current;
}

void Adsr::connectNodes() { envelope >> blackHole; }

} // namespace Clover::NodeSimplex::Envelope