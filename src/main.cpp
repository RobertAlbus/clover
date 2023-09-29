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

#define NUM_SECONDS (100)

#include <cmath>
#include <csignal> // midi input message demo

#include <chrono>
#include <cstdlib> // rand
#include <math.h>
#include <numbers>
#include <thread>

#include "portaudio.h"

#include "Clover.h"

#include "NxOscStsqDemo.h"

using namespace Clover::IO;
using namespace Clover::Nodes;

Interface *globalInterface = nullptr;
extern "C" void handleInterrupt(int sig) {
  if (globalInterface != nullptr) {
    globalInterface->stop();
    globalInterface->close();
    globalInterface->terminate();
  }
  globalInterface = 0;
  exit(0);
}

int main(int argc, char *argv[]) {
  ////////////////////////////////////////////////////////////////
  // CLOVER POC

  Clover::IO::MidiIn::printPorts();

  Interface interface;
  globalInterface = &interface;
  signal(SIGINT, handleInterrupt);

  interface.rootNode.gain(0.5);
  Clover::Util::Time time(160, Clover::Base::sampleRate, &interface.clock);
  Clover::Nodes::Adapter::NullInAdapter<2> nullSink;
  Clover::Nodes::Adapter::NullOutAdapter<1> blackHole1;

  blackHole1 >> nullSink >> interface.rootNode;

  Wavetable::WavetableOscStereo osc;
  osc.saw(1024);
  osc.freq(70.);
  osc.stereoDetune(-0.005f);

  Wavetable::WavetableOsc mod;
  mod.sine(11);
  mod.freq(71);

  mod >> blackHole1;

  Filter::Filter<2> filter;
  filter.lowPass();
  filter.set(200., 0.9);

  Filter::EQ<2> EQ;
  EQ.lowShelf();
  EQ.set(80., 0.7, -10);

  osc >> filter >> EQ >> interface.rootNode;
  EQ.gain(0.33);

  Wavetable::WavetableOsc lfo;
  lfo.sine(1024);
  lfo.freq(0.25);
  lfo.phase(0.75);
  lfo >> blackHole1;

  Envelope::ADSR adsr(time.quat(0.22), time.quat(), 0.0f, time.beat());
  adsr >> blackHole1;

  OscAndStSq testPattern(time);

  testPattern.instrument >> interface.rootNode;
  testPattern.kick >> interface.rootNode;
  testPattern.stsq_pitch >> nullSink;
  testPattern.stsq_kick >> nullSink;
  testPattern.stsq_trigger >> nullSink;

  bool isProfilingMode = false;
  if (isProfilingMode) {
    int quantity = Clover::Base::sampleRate * 360 * 10;
    std::vector<Clover::Graph::AudioFrame<2>> benchmarkData;
    benchmarkData.reserve(quantity);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < quantity; i++) {
      interface.rootNode.metaTick(interface.clock.currentSample());
      interface.clock.tick();
      benchmarkData.emplace_back(interface.rootNode.frames.current);
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = duration_cast<std::chrono::milliseconds>(end - start);
    printf("\n\nTHIS MANY   %i\n\n", static_cast<int>(duration.count()));
    exit(0);
  } else {
    interface.clock.registerTickCallback([&](int currentTime) -> void {
      float lfoAdjusted = lfo.frames.current[0] + 1.;
      float modAdjusted = (mod.frames.current[0] + 1.) / 2.;
      float oscAdjusted = (osc.frames.current[0] + 1.) / 2.;
      float envelopeValue = adsr.frames.current[0];
      osc.freq(100. * (modAdjusted * 2. - 1));
      mod.freq(400. * (oscAdjusted * 3.));

      float cut = envelopeValue * (1000. * lfoAdjusted) + 200;
      float reso = envelopeValue * (-1. * lfoAdjusted) + 2;
      filter.set(cut, reso);
      osc.gain(envelopeValue);

      float currentQuat = time.currentQuat() + 3.f;

      float currentSixteenthInTwoBeats = fmod(currentQuat, 8.f);
      if (currentSixteenthInTwoBeats == 0.f ||
          currentSixteenthInTwoBeats == 3.f ||
          currentSixteenthInTwoBeats == 6.f) {
        osc.phase(0);
        mod.phase(0);
        adsr.triggerOn();
      }

      float currentBar = fmod(time.currentBar(), 16);
      if (currentBar == 0 && time.currentUnit(1) != 0) {
        testPattern.stsq_pitch.setPattern(0);
      } else if (currentBar == 12) {
        testPattern.stsq_pitch.setPattern(1);
      }
    });

    if (interface.initialize() != paNoError)
      return 1;

    interface.hostInfo();

    if (interface.openDevice(Pa_GetDefaultOutputDevice()) != paNoError)
      return 1;

    // might need to add some sort of "graphReady" functionaltiy to prevent
    // starting until everything is initialized
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (interface.start() != paNoError) {
      interface.close();
      return 1;
    }

    // TODO: need a way to determine the composition length but this will do for
    // now. I should be able to compute this value once I have composition-level
    // utilities.
    std::this_thread::sleep_for(std::chrono::seconds(NUM_SECONDS));

    interface.stop();
    return paNoError;
  }
}
