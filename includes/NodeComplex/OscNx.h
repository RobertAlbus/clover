#pragma once

#include "Clover.h"
#include "Graph.h"
#include "NodeSimplex.h"

using namespace Clover::NodeSimplex;

template <int __numVoices>
struct OscNx : public Clover::Graph::AudioNode<0, 2>, Pitchable, Triggerable {
  struct Voice {
    Voice() : tuning_(0) {}

    void phase(float phase) { osc.phase(phase); }

    void note(float midiNote) {}
    float note() { return osc.note(); }

    void tuning(float semitones) { tuning_ = semitones; }
    float tuning() { return tuning_; }

    void stereoDetune(float semitones) { osc.stereoDetune(semitones); }

    void gain(float g) { osc.gain(g); }

    void
    wavetable(std::shared_ptr<Clover::NodeSimplex::Wavetable::Wavetable> wt) {
      osc.wavetable(wt);
    }

    std::shared_ptr<Clover::NodeSimplex::Wavetable::Wavetable> wavetable() {
      return osc.wavetable();
    }

    void sine(int size = 512) { osc.sine(size); }
    void square(int size = 512) { osc.square(size); }
    void saw(int size = 512) { osc.saw(size); }
    void tri(int size = 512) { osc.tri(size); }
    void noise(int size = 1024) { osc.noise(size); }

  private:
    friend OscNx;
    Clover::NodeSimplex::Wavetable::WavetableOscStereo osc;
    float tuning_;
  };

  struct Mod {
    Mod() {
      adsr.gain(0);
      lfo.gain(0);
    }

    Clover::NodeSimplex::Envelope::Adsr adsr;
    Clover::NodeSimplex::Wavetable::WavetableOsc lfo;

    float adsrValue() { return adsr.currentFrame()[0]; }

    float lfoValue() { return lfo.currentFrame()[0]; }
  };

  OscNx() {
    connectNodes();
    amplitude.adsr.gain(1);
  }

  void pan(float p) { pan_.pan(p); }

  void midiNote(float note) {
    currentMidiNote = note;
    currentMidiNoteFreq = Clover::Util::Calc::mtof(note);
  }

  void triggerOn() {
    amplitude.adsr.triggerOn();
    filterCut.adsr.triggerOn();
    filterQ.adsr.triggerOn();
    pitch.adsr.triggerOn();

    amplitude.lfo.phase(0);
    filterCut.lfo.phase(0);
    filterQ.lfo.phase(0);
    pitch.lfo.phase(0);
  }

  void triggerOff() {
    amplitude.adsr.triggerOff();
    filterCut.adsr.triggerOff();
    filterQ.adsr.triggerOff();
    pitch.adsr.triggerOff();
  }

  std::array<Voice, __numVoices> voices;
  Clover::NodeSimplex::Filter::Filter<2> filter;
  Clover::NodeSimplex::Stereo::Pan2 pan_;

  Mod amplitude;
  Mod filterCut;
  Mod filterQ;
  Mod pitch;
  Clover::NodeSimplex::Filter::Filter<4> modSmoothing;

  float filterCutoff_ = 21000.f;
  float filterReso_ = 0.707f;
  float currentMidiNote;

  float currentMidiNoteFreq;

  Clover::NodeSimplex::Adapter::NullAdapter<1, 0> blackHole1;
  Clover::NodeSimplex::Adapter::NullAdapter<2, 0> blackHole2;
  Clover::NodeSimplex::Basic::Gain<2> audioSink;

  AudioFrame<2> tick(AudioFrame<0> input) {
    updatePitchMod();
    updateFilterMod();
    updateAmplitudeMod();

    AudioFrame<2> f{audioSink.currentFrame()[0], audioSink.currentFrame()[1]};

    float ff = filter.freq();
    float fr = filter.reso();

    return f;
  }

  void connectNodes() {
    for (Voice &voice : voices)
      voice.osc >> filter;
    filter >> pan_ >> audioSink >> blackHole2 >> this;

    amplitude.adsr >> blackHole1;
    amplitude.lfo >> blackHole1;

    filterCut.adsr >> blackHole1;
    filterCut.lfo >> blackHole1;

    filterQ.adsr >> blackHole1;
    filterQ.lfo >> blackHole1;

    pitch.adsr >> blackHole1;
    pitch.lfo >> blackHole1;

    blackHole1 >> this;
  }

  void updatePitchMod() {
    float pitchMod = pitch.adsrValue() + pitch.lfoValue();

    for (Voice &voice : voices) {
      float voiceFreq = Clover::Util::Calc::freqBySemitoneDifference(
          currentMidiNoteFreq, voice.tuning());
      voice.osc.freq(voiceFreq + pitchMod);
    }
  }

  void updateFilterMod() {
    // float fff = getLogScaledFreq(filterCutoff_, filterCut.adsr.gain(),
    // filterCut.adsrValue());

    float filterCutoff =
        filterCutoff_ + filterCut.adsrValue() + filterCut.lfoValue();

    float filterReso = filterReso_ + filterQ.adsrValue() + filterQ.lfoValue();
    filter.set(filterCutoff, filterReso);
  }

  void updateAmplitudeMod() {
    audioSink.gain(amplitude.adsrValue() + amplitude.lfoValue());
  }
};
