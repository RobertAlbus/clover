#pragma once

#include <vector>

#include "Clover.h"

using namespace Clover::Midi::Note;


struct OscAndStSq {

  OscAndStSq(Clover::Util::Time& time) {
    patchTheSynth(time);

    std::vector<STSQ_Pattern<float>> notePatterns = getNotePatterns(time);
    stsq_pitch.addPattern(notePatterns[0]);
    stsq_pitch.addPattern(notePatterns[1]);

    std::vector<STSQ_Pattern<TriggerState>> triggerPatterns = getTriggerPatterns(time);
    stsq_trigger.addPattern(triggerPatterns[0]);
    stsq_trigger.addPattern(triggerPatterns[1]);

    stsq_pitch.targets.emplace_back(&instrument);
    stsq_trigger.targets.emplace_back(&instrument);
  }

  std::vector<STSQ_Pattern<float>> getNotePatterns(Clover::Util::Time& time) {
    std::vector<STSQ_Pattern<float>> patterns;

    // clang-format off
    STSQ_Pattern<float> pattern1;
    pattern1.totalDuration = time.beat(4);
    pattern1.add(G2, time.beat(0.0));
    pattern1.add(F2 , time.beat(1.5));
    pattern1.add(As2, time.beat(2.5));
    pattern1.add(Gs2, time.beat(3.0));

    STSQ_Pattern<float> pattern2;
    pattern2.totalDuration = time.beat(4);
    pattern2.add(Fs4, time.beat(0.0));
    pattern2.add(C6 , time.beat(1.5));
    pattern2.add(As5, time.beat(2.5));
    pattern2.add(Cs5, time.beat(3.0));
    // clang-format on

    patterns.emplace_back(pattern1);
    patterns.emplace_back(pattern2);

    return patterns;
  }

  std::vector<STSQ_Pattern<TriggerState>> getTriggerPatterns(Clover::Util::Time& time) {
    std::vector<STSQ_Pattern<TriggerState>> patterns;

    // clang-format off
    STSQ_Pattern<TriggerState> pattern1;
    pattern1.totalDuration = time.beat(16);
    pattern1.add(TriggerState::On,  time.beat(0.0));
    pattern1.add(TriggerState::On, time.beat(2.0) - 1);
    pattern1.add(TriggerState::On,  time.beat(4.0));
    pattern1.add(TriggerState::On, time.beat(6.0) - 1);
    pattern1.add(TriggerState::On,  time.beat(8.0));
    pattern1.add(TriggerState::Off, time.beat(10.0));
    pattern1.add(TriggerState::On,  time.beat(12.0));
    pattern1.add(TriggerState::Off, time.beat(14.0));

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
    instrument.voices[1].tuning(0);
    instrument.voices[2].tuning(7);

    instrument.voices[0].gain(1.);
    instrument.voices[1].gain(0.5);
    instrument.voices[2].gain(0.8);

    instrument.filter.lowPass();
    instrument.filterCutoff_ = 100;
    instrument.filterCut.adsr.set(time.beat(1), 0, 1, time.beat());
    instrument.filterCut.adsr.gain(1000);

    instrument.amplitude.adsr.set(time.quat(1),0,1,time.quat(2));
    instrument.gain(0.6);
  }

  OscNx<3> instrument;
  STSQ_Pitchable stsq_pitch;
  STSQ_Triggerable stsq_trigger;
};