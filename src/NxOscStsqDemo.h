#pragma once

#include <vector>

#include "Clover.h"

using namespace Clover::Midi::Note;


struct OscAndStSq {

  OscAndStSq(Clover::Util::Time& time) {
    patchTheSynth(time);
    patchTheKick(time);

    std::vector<STSQ_Pattern<float>> notePatterns = getNotePatterns(time);
    stsq_pitch.addPattern(notePatterns[0]);
    stsq_pitch.addPattern(notePatterns[1]);

    std::vector<STSQ_Pattern<TriggerState>> triggerPatterns = getTriggerPatterns(time);
    stsq_trigger.addPattern(triggerPatterns[0]);
    stsq_trigger.addPattern(triggerPatterns[1]);

    stsq_kick.addPattern(getKickPatterns(time));
    kick.midiNote(G2 - 0.1);

    stsq_pitch.targets.emplace_back(&instrument);
    stsq_trigger.targets.emplace_back(&instrument);

    stsq_kick.targets.emplace_back(&kick);
  }

  std::vector<STSQ_Pattern<float>> getNotePatterns(Clover::Util::Time& time) {
    std::vector<STSQ_Pattern<float>> patterns;

    // clang-format off
    STSQ_Pattern<float> pattern1;
    pattern1.totalDuration = time.beat(4);
    pattern1.add(As2, time.beat(0.0));
    pattern1.add(Cs3, time.beat(1.0));
    pattern1.add(Gs3, time.beat(2.1));
    pattern1.add(As3, time.beat(3.0));

    STSQ_Pattern<float> pattern2;
    pattern2.totalDuration = time.beat(2);
    pattern2.add(As3, time.beat(0.0));
    pattern2.add(Cs4, time.beat(0.5));
    pattern2.add(Gs4, time.beat(1.0));
    pattern2.add(As4, time.beat(1.5));

    // clang-format on

    patterns.emplace_back(pattern1);
    patterns.emplace_back(pattern2);

    return patterns;
  }

  STSQ_Pattern<TriggerState> getKickPatterns(Clover::Util::Time& time) {

    STSQ_Pattern<TriggerState> pattern1;
    pattern1.totalDuration = time.beat(4);
    pattern1.add(TriggerState::On, time.quat(0.0));
    pattern1.add(TriggerState::On, time.quat(3.0));
    pattern1.add(TriggerState::On, time.quat(6.0));
    pattern1.add(TriggerState::On, time.quat(8.0));
    pattern1.add(TriggerState::On, time.quat(11.0));
    pattern1.add(TriggerState::On, time.quat(13.0));

    return pattern1;
  }

  std::vector<STSQ_Pattern<TriggerState>> getTriggerPatterns(Clover::Util::Time& time) {
    std::vector<STSQ_Pattern<TriggerState>> patterns;

    // clang-format off
    STSQ_Pattern<TriggerState> pattern1;
    pattern1.totalDuration = time.quat(16);
    pattern1.add(TriggerState::On,  time.quat(0.0));
    pattern1.add(TriggerState::Off,  time.quat(1.0));
    pattern1.add(TriggerState::On,  time.quat(5.0));
    pattern1.add(TriggerState::Off,  time.quat(7.0));
    pattern1.add(TriggerState::On,  time.quat(8.0));
    pattern1.add(TriggerState::Off,  time.quat(11.0));

    STSQ_Pattern<TriggerState> pattern2;
    pattern2.totalDuration = time.beat(8);
    pattern2.add(TriggerState::On, time.beat(0.0));
    // clang-format on

    patterns.emplace_back(pattern1);
    patterns.emplace_back(pattern2);

    return patterns;
  }

  void patchTheSynth(Clover::Util::Time& time) {
    instrument.voices[0].sine();
    instrument.voices[1].square();
    instrument.voices[2].square();

    instrument.voices[0].gain(1);
    instrument.voices[1].gain(0.7);
    instrument.voices[2].gain(0.5);

    instrument.voices[1].stereoDetune(0.2);
    instrument.voices[2].stereoDetune(-0.2);

    instrument.voices[0].tuning(-12);
    instrument.voices[1].tuning(0.1);
    instrument.voices[2].tuning(7.15);

    instrument.voices[0].gain(1.);
    instrument.voices[1].gain(0.5);
    instrument.voices[2].gain(0.8);

    instrument.filter.lowPass();
    instrument.filterCutoff_ = 10;
    instrument.filterReso_ = 2;
    instrument.filterCut.adsr.set(time.beat(0.5), time.beat(0.1), 0.5, time.beat(2));
    instrument.filterCut.adsr.gain(4000);

    instrument.amplitude.adsr.set(time.quat(0.8),0,1,time.beat(2));
    instrument.gain(0.1);
  }

  void patchTheKick(Clover::Util::Time& time) {
    kick.voices[0].sine();
    kick.voices[1].sine();
    kick.voices[2].sine();

    kick.voices[0].gain(1);
    kick.voices[1].gain(0.7);
    kick.voices[2].gain(0.5);

    kick.voices[1].stereoDetune(0.02);
    kick.voices[2].stereoDetune(-0.01);

    kick.voices[0].tuning(-9);
    kick.voices[1].tuning(5);
    kick.voices[2].tuning(11);

    kick.voices[0].gain(1.);
    kick.voices[1].gain(0.5);
    kick.voices[2].gain(0.8);

    kick.filter.lowPass();
    kick.filterCutoff_ = 10;
    kick.filterReso_ = .7f;
    kick.pitch.adsr.set(0, time.quat(0.1f), 0, 0);
    kick.pitch.adsr.gain(800);
    kick.filterCut.adsr.set(50, time.quat(0.2f), 0.02f, 0);
    kick.filterCut.adsr.gain(1000);

    kick.amplitude.adsr.set(50, time.quat(1.8f), 0.02f, 0);
    kick.gain(0.9);
  }

  OscNx<3> instrument;
  STSQ_Pitchable stsq_pitch;
  STSQ_Triggerable stsq_trigger;

  STSQ_Triggerable stsq_kick;
  
  OscNx<3> kick;
};