#pragma once

#include "Algo/Envelope/ADSR.h"
#include "Graph.h"
#include "NodeSimplex/StepSequencer/lib.h"

namespace Clover::NodeSimplex::Envelope {

class ADSR : public Triggerable, public AudioNode<0, 1> {
public:
  ADSR();
  ADSR(int a, int d, float s, int r);

  void set(float a, float d, float s, float r);
  void set(int a, int d, float s, int r);

  void attack(int a);
  void decay(int d);
  void sustain(float s);
  void release(int r);

  int attack();
  int decay();
  float sustain();
  int release();

  void triggerOn() override;
  void triggerOff() override;

protected:
  Clover::Envelope::ADSR<float> envelope;

  AudioFrame<1> tick(AudioFrame<0> input);
};

} // namespace Clover::NodeSimplex::Envelope