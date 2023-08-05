#define NUM_SECONDS (100)

#include <cmath>
#include <csignal> // midi input message demo

#include <chrono>
#include <cstdlib> // rand
#include <math.h>
#include <numbers>
#include <thread>

#include "RtMidi.h"
#include "portaudio.h"

#include "Config.h"

#include "Clover.h"

#include "NxOscStsqDemo.h"

using namespace Clover::IO;
using namespace Clover::NodeSimplex;

float getRandomFloat(int max) {
  return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / max));
}

int main(int argc, char *argv[]) {
  ////////////////////////////////////////////////////////////////
  // CLOVER POC

  printf("\nClover Version %d.%d\n", Clover_VERSION_MAJOR,
         Clover_VERSION_MINOR);
  printf("\nDefault Audio Device Index: %d\n", Pa_GetDefaultOutputDevice());

  Interface interface;
  interface.rootNode.gain(0.5);
  Clover::Util::Time time(160, Clover::Base::sampleRate, &interface.clock);
  Clover::NodeSimplex::Adapter::NullAdapter<1, 2> blackHole;
  blackHole >> interface.rootNode;

  Wavetable::WavetableOscStereo osc;
  osc.saw(1024);
  osc.freq(70.);
  osc.stereoDetune(-0.005f);

  Wavetable::WavetableOsc mod;
  mod.sine(11);
  mod.freq(71);

  mod >> blackHole;

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
  lfo >> blackHole;

  Envelope::ADSR adsr(time.quat(0.22), time.quat(), 0.0f, time.beat());
  adsr >> blackHole;

  OscAndStSq testPattern(time);

  Adapter::NullAdapter<0, 2> nullAdapter;

  testPattern.instrument >> interface.rootNode;
  testPattern.kick >> interface.rootNode;
  testPattern.stsq_pitch >> nullAdapter >> interface.rootNode;
  testPattern.stsq_kick >> nullAdapter;
  testPattern.stsq_trigger >> nullAdapter;

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
    printf("\n\nTHIS MANY   %i\n\n", duration);
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
