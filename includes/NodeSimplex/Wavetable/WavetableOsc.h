#pragma once
#include <cmath>
#include <functional>
#include <memory>
#include <tgmath.h>
#include <vector>

#include "Constants.h"
#include "Util.h"

namespace Clover::NodeSimplex::Wavetable {

typedef std::vector<Sample> Wavetable;

struct WavetableOscSettings {
  float freq;
  float phase;
  float phaseOffset;
  float readIndex;
  float readIndexIncrement;
  float readIndexOffset;
  std::shared_ptr<Wavetable> wavetable;
  float wavetableSize;
};

class WavetableOsc : public StatefulProcessor<0, 1, WavetableOscSettings> {
public:
  WavetableOsc()
      : WavetableOsc(std::make_shared<std::vector<Sample>>(
                         Util::GenerateWavetable::Sine(256)),
                     200, 0, 0) {}

  WavetableOsc(std::shared_ptr<Wavetable> wavetable, float freq,
               float phase = 0, float phaseOffset = 0)
      : StatefulProcessor<0, 1, WavetableOscSettings>() {
    settings.initial.wavetable = wavetable;
    settings.initial.wavetableSize = (float)wavetable->size();
    settings.initial.freq = freq;
    settings.initial.phase = normalizePhase(phase);
    settings.initial.phaseOffset = normalizePhase(phaseOffset);

    settings.initial.readIndexIncrement = calculateReadIndexIncrement(freq);
    settings.initial.readIndex = 0.;
    settings.initial.readIndexOffset =
        calculateReadIndexOffset(settings.initial.phaseOffset);

    settings.reset();
  }

  void phase(float phase) {
    phase = normalizePhase(phase);
    settings.current.phase = phase;
    settings.current.readIndex = (settings.current.wavetableSize) * phase;
  }

  float phase() {
    return settings.current.phase / settings.current.wavetableSize;
  }

  void phaseOffset(float offset) {
    settings.current.phaseOffset = normalizePhase(offset);
    settings.current.readIndexOffset = calculateReadIndexOffset(offset);
  }

  float phaseOffset() { return settings.current.phaseOffset; }

  void freq(float freq) {
    freq = fabs(freq);
    settings.current.freq = freq;
    settings.current.readIndexIncrement = calculateReadIndexIncrement(freq);
  }

  float freq() { return settings.current.freq; }

  void wavetable(std::shared_ptr<Wavetable> wt) {
    settings.current.wavetable = wt;
    settings.current.wavetableSize = (float)wt->size();
    settings.current.readIndexIncrement =
        calculateReadIndexIncrement(settings.current.freq);
  }

  std::shared_ptr<Wavetable> wavetable() { return settings.current.wavetable; }

  void sine(int size = 512) {
    std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
        Util::GenerateWavetable::Sine(size));
    wavetable(wt);
  }
  void square(int size = 512) {
    std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
        Util::GenerateWavetable::Square(size));
    wavetable(wt);
  }
  void saw(int size = 512) {
    std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
        Util::GenerateWavetable::Saw(size));
    wavetable(wt);
  }
  void tri(int size = 512) {
    std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
        Util::GenerateWavetable::Tri(size));
    wavetable(wt);
  }
  void noise(int size = 1024) {
    std::shared_ptr<Wavetable> wt = std::make_shared<std::vector<Sample>>(
        Util::GenerateWavetable::NoiseWhite(size));
    wavetable(wt);
  }

private:
  Frame<1> tick(Frame<0> input) {
    float direction = Clover::Util::Calc::sign(settings.current.freq);
    float nextIndex =
        normalizeReadIndex(floor(settings.current.readIndex) + direction);

    Wavetable &wt = *(settings.current.wavetable);

    // this lerp is technically incorrecto for negative frequencies. Should fix
    // that.
    float value =
        std::lerp(wt[(int)settings.current.readIndex], wt[(int)nextIndex],
                  fmod(settings.current.readIndex, 1)

        );

    Frame<1> f{value};

    float newIndex = settings.current.readIndex +
                     settings.current.readIndexOffset +
                     settings.current.readIndexIncrement;

    settings.current.readIndex = normalizeReadIndex(newIndex);

    return f;
  }

  float normalizeReadIndex(float index) {
    float wtSize = (float)settings.current.wavetableSize;
    return fmod(fmod(index, wtSize) + wtSize, wtSize);
  }

  float normalizePhase(float phase) { return fmod(fmod(phase, 1.) + 1., 1); }

  float calculateReadIndexIncrement(float freq) {
    return freq * ((float)settings.current.wavetableSize) /
           ((float)SAMPLE_RATE);
  }

  float calculateReadIndexOffset(float phaseOffset) {
    return ((float)settings.current.wavetableSize) *
           normalizePhase(phaseOffset);
  }

public:
  void printSettings(WavetableOscSettings &settings) {
    printf("\n%f   - %f   - %f   - %f   - %f   - %f   - %f\n",
           settings.readIndex, settings.readIndexIncrement,
           settings.wavetableSize, settings.freq, settings.phase,
           settings.phaseOffset, settings.readIndexOffset);
  }
};

} // namespace Clover::NodeSimplex::Wavetable