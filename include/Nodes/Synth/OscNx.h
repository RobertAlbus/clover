#pragma once

/*
 * /////////
 * // Clover
 *
 * Audio processing algorithms and DAG with feedback loops that do not break
 * acyclicity.
 *
 * Copyright (C) 2023 Rob W. Albus
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Algorithm/Math.h"
#include "Graph/AudioFrame.h"
#include "Graph/AudioNode.h"
#include "Graph/NullFrame.h"
#include "Nodes/Adapter/NullAdapter.h"
#include "Nodes/Basic/Gain.h"
#include "Nodes/Envelope/ADSR.h"
#include "Nodes/Filter/Filter.h"
#include "Nodes/StepSequencer/lib.h"
#include "Nodes/Stereo/Pan.h"
#include "Nodes/Wavetable/WavetableOsc.h"
#include "Nodes/Wavetable/WavetableOscInterface.h"
#include "Nodes/Wavetable/WavetableOscStereo.h"
#include "Util/Calc.h"

namespace Clover::Nodes::Synth {

template <int __numVoices>
struct OscNx : public Clover::Graph::AudioOutNode<2>, Pitchable, Triggerable {
  struct Voice {
    Voice() : tuning_(0) {}

    void phase(float phase) { osc.phase(phase); }

    void note(float midiNote) {}
    float note() { return osc.note(); }

    void tuning(float semitones) { tuning_ = semitones; }
    float tuning() { return tuning_; }

    void stereoDetune(float semitones) { osc.stereoDetune(semitones); }

    void gain(float g) { osc.gain(g); }

    void wavetable(std::shared_ptr<Wavetable::Wavetable> wt) {
      osc.wavetable(wt);
    }

    std::shared_ptr<Wavetable::Wavetable> wavetable() {
      return osc.wavetable();
    }

    void sine(int size = 512) { osc.sine(size); }
    void square(int size = 512) { osc.square(size); }
    void saw(int size = 512) { osc.saw(size); }
    void tri(int size = 512) { osc.tri(size); }
    void noise(int size = 1024) { osc.noise(size); }

  private:
    friend OscNx;
    Wavetable::WavetableOscStereo osc;
    float tuning_;
  };

  struct Mod {
    Mod() {
      adsr.gain(0);
      lfo.gain(0);
    }

    Clover::Nodes::Envelope::ADSR adsr;
    Clover::Nodes::Wavetable::WavetableOsc lfo;

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
    currentMidiNoteFreq = Clover::Algorithm::midi_to_frequency(note);
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
  Clover::Nodes::Filter::Filter<2> filter;
  Clover::Nodes::Stereo::Pan2 pan_;

  Mod amplitude;
  Mod filterCut;
  Mod filterQ;
  Mod pitch;
  Clover::Nodes::Filter::Filter<4> modSmoothing;

  float filterCutoff_ = 21000.f;
  float filterReso_ = 0.707f;
  float currentMidiNote;

  float currentMidiNoteFreq;

  Clover::Nodes::Adapter::NullOutAdapter<1> blackHole1;
  Clover::Nodes::Adapter::NullOutAdapter<2> blackHole2;
  Clover::Nodes::Basic::Gain<2> audioSink;

  Clover::Graph::AudioFrame<2> tick(Clover::Graph::NullFrame input) {
    updatePitchMod();
    updateFilterMod();
    updateAmplitudeMod();

    Clover::Graph::AudioFrame<2> f{
        audioSink.currentFrame()[0], audioSink.currentFrame()[1]};

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
    float pitchModOctaves = pitch.adsrValue() + pitch.lfoValue();

    for (Voice &voice : voices) {
      float voiceFreq = Algorithm::frequency_by_semitone_difference(
          currentMidiNoteFreq, voice.tuning()
      );
      float voiceFreqPitchModHz =
          Algorithm::frequency_by_octave_difference(voiceFreq, pitchModOctaves);

      voice.osc.freq(voiceFreqPitchModHz);
    }
  }

  void updateFilterMod() {
    // octave range from cut off

    float currentModValueOctaves =
        (filterCut.adsrValue() + filterCut.lfoValue());

    float modAmountHz =
        Algorithm::frequency_by_octave_difference(10.f, currentModValueOctaves);
    float filterCutoff = filterCutoff_ + modAmountHz;

    float filterReso = filterReso_ + filterQ.adsrValue() + filterQ.lfoValue();

    filter.set(filterCutoff, filterReso);
  }

  void updateAmplitudeMod() {
    audioSink.gain(amplitude.adsrValue() + amplitude.lfoValue());
  }
};

} // namespace Clover::Nodes::Synth
