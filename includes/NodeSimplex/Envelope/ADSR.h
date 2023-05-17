#pragma once

#include <algorithm>
#include <cmath>

#include "Graph.h"
#include "NodeSimplex.h"
#include "NodeSimplex/StepSequencer/lib.h"

namespace Clover::NodeSimplex::Envelope {

struct AdsrSettings {
  AdsrSettings(size_t a = 0, size_t d = 0, float s = 1, size_t r = 0);

  size_t attack;
  size_t decay;
  float sustain;
  size_t release;
  size_t startTime;
  bool keyOn;
};

class Adsr : public Triggerable, public StatefulSubgraph<0, 1, AdsrSettings> {
public:
  Adsr(size_t a = 0, size_t d = 0, float s = 1, size_t r = 0);
  Adsr(AdsrSettings initialSettings);

  void set(size_t a, size_t d, float s, size_t r);
  void set(float a, float d, float s, float r);

  size_t attack();
  void attack(size_t a);

  size_t decay();
  void decay(size_t d);

  float sustain();
  void sustain(float s);

  size_t release();
  void release(size_t r);

  // todo: deprecate
  void keyOn();
  void keyOff();

  void triggerOn() override;
  void triggerOff() override;

protected:
  BasicEnvelope envelope;

  Frame<1> tick(Frame<0> input);
  void connectNodes();
};

} // namespace Clover::NodeSimplex::Envelope