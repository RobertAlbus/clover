#include "Graph.h"
#include "NodeSimplex/Envelope/ADSR.h"

namespace Clover::NodeSimplex::Envelope {

ADSR::ADSR()
    : ADSR(1, 1, 1.f, 1) {
}

ADSR::ADSR(int a, int d, float s, int r)
    : AudioNode(), envelope(a, d, s, r) {

}

void ADSR::set(int a, int d, float s, int r) {
  attack(a);
  decay(d);
  sustain(s);
  release(r);
}

void ADSR::set(float a, float d, float s, float r) {
  int a_rounded = static_cast<int>(std::round(a));
  int d_rounded = static_cast<int>(std::round(a + d) - a_rounded);
  int r_rounded = static_cast<int>(std::round(r));

  attack(a_rounded);
  decay(d_rounded);
  sustain(s);
  release(r_rounded);
}

int ADSR::attack() { return envelope.attack(); }
void ADSR::attack(int a) { envelope.attack(a); }

int ADSR::decay() { return envelope.decay(); }
void ADSR::decay(int d) { envelope.decay(d); }

float ADSR::sustain() { return envelope.sustain(); }
void ADSR::sustain(float s) { envelope.sustain(s); }

int ADSR::release() { return envelope.release(); }
void ADSR::release(int r) { envelope.release(r); }

void ADSR::triggerOn() {
  envelope.keyOn();
}
void ADSR::triggerOff() {
  envelope.keyOff();
}

AudioFrame<1> ADSR::tick(AudioFrame<0> input) {
  return AudioFrame<1> {envelope.process()};
}

} // namespace Clover::NodeSimplex::Envelope