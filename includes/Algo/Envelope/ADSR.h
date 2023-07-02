#pragma once

#include <functional>

#include "Algo/AlgorithmBase.h"
#include "Algo/Envelope/Basic.h"
#include "Util/FloatingPointConcept.h"

namespace Clover::Envelope {

template <FloatingPoint T> struct ADSR : public AlgorithmBase<T> {
public:
  ADSR() : ADSR(2u, 2u, T(1), 2u) {}

  ADSR(int a, int d, T s, int r) : velocity_(T(1)), isKeyOn(false) {
    set(a, d, s, r);
  }

  void set(int a, int d, T s, int r) {
    attack(a);
    decay(d);
    sustain(s);
    release(r);
  }

  int attack() { return attackSamples; }
  void attack(int newDuration) {
    attackSamples = std::max(newDuration, 0);
    if (isAttackPhase()) {
      int remainingTimeInNewDuration =
          std::max(attackSamples - elapsedSamples, 0);
      envelope.set(attackRange, remainingTimeInNewDuration);
    }

    attackSamples = newDuration;
  }

  int decay() { return decaySamples; }
  void decay(int newDuration) {
    decaySamples = std::max(newDuration, 0);
    if (isDecayPhase()) {
      int elapsedDecaySamples = elapsedSamples - attackSamples;
      int remainingTimeInNewDuration =
          std::max(decaySamples - elapsedDecaySamples, 0);
      envelope.set(sustainValue, remainingTimeInNewDuration);
    }
  }

  T sustain() { return sustainValue; }
  void sustain(T newValue) {
    sustainValue = newValue;
    // initiate decay update with same duration but new sustain value
    decay(decay());

    if (isSustainPhase()) {
      envelope.set(sustainValue, 1);
    }
  }

  int release() { return releaseSamples; }
  void release(int newDuration) {
    releaseSamples = std::max(newDuration, 0);
    if (isReleasePhase()) {
      int remainingTimeInNewDuration =
          std::max(releaseSamples - elapsedSamples, 0);
      envelope.set(T(0), remainingTimeInNewDuration);
    }
  }

  void keyOn(T velocity = T(1)) {
    velocity_ = velocity;
    isKeyOn = true;
    elapsedSamples = -1;
    envelope.set(0, attackRange, attackSamples);
  }

  void keyOff() {
    isKeyOn = false;
    elapsedSamples = -1;
    envelope.set(0u, releaseSamples);
  }

  void onComplete(std::function<void(const ADSR<T> *)> callback) {
    onCompleteCallback = callback;
  }

  T process() {
    elapsedSamples++;

    if (hasCompleted())
      onCompleteCallback(this);

    if (isDecayStart()) {
      envelope.set(sustainValue, decaySamples);
    }

    this->processed = envelope.process() * velocity_;
    return this->processed;
  }

protected:
  T attackRange = T(1);
  int attackSamples;
  T sustainValue;
  int decaySamples;
  int releaseSamples;

  int elapsedSamples;
  bool isKeyOn;
  T velocity_;

  std::function<void(const ADSR<T> *)> onCompleteCallback =
      [](const ADSR<T> *self) { return; };

  Clover::Envelope::Basic<T> envelope;

private:
  int atLeastOne(int value) const { return value > 0 ? value : 1; }

  bool isAttackPhase() const {
    return isKeyOn && elapsedSamples <= attackSamples;
  }

  bool isDecayPhase() const {
    return isKeyOn && elapsedSamples > attackSamples &&
           elapsedSamples <= attackSamples + decaySamples;
  }

  bool isDecayStart() const {
    return isKeyOn && elapsedSamples == attackSamples;
  }

  bool isSustainPhase() const {
    return isKeyOn && elapsedSamples >= attackSamples + decaySamples;
  }

  bool isReleasePhase() const {
    return !isKeyOn && elapsedSamples <= releaseSamples;
  }

  bool hasCompleted() const {
    return !isKeyOn && elapsedSamples == releaseSamples - 1;
  }
};

} // namespace Clover::Envelope
