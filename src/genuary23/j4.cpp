#define NUM_SECONDS (12)

#include <cmath>
#include <csignal> // midi input message demo

#include <chrono>
#include <cstdlib> // rand
#include <math.h>
#include <numbers>
#include <thread>
#include <vector>

#include "RtMidi.h"
#include "portaudio.h"

#include "Config.h"

#include "Clover.h"

#include <bits/stdc++.h>

using namespace Clover::IO;
using namespace Clover::NodeSimplex;

float getRandomFloat(int max) {
  return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / max));
}

int main(int argc, char *argv[]) {
  Interface interface;
  Time time(160, SAMPLE_RATE, &interface.clock);
  Clover::NodeSimplex::Adapter::NullAdapter<1, 2> blackHole;
  blackHole >> interface.rootNode;

  std::string wavPath = "/home/trax/code/music/clover/renders/j4.wav";
  Clover::IO::WavFile<2> wavOut(wavPath, 480000 * 10);
  wavOut >> *(new Adapter::NullAdapter<0, 1>()) >> blackHole;

  int numOscs = 6;
  std::vector<Clover::NodeSimplex::Wavetable::WavetableOsc *> oscs;
  std::vector<Clover::NodeSimplex::Envelope::BasicEnvelope *> envs;
  Clover::NodeSimplex::Stereo::Pan1 pan;
  pan.gain(0.3);
  pan >> interface.rootNode;
  pan >> wavOut;

  std::default_random_engine gen;
  gen.seed(0);
  std::uniform_real_distribution<float> distribution(0.0, 1000.0);

  for (int i = 0; i < numOscs; i++) {
    oscs.emplace_back(new Clover::NodeSimplex::Wavetable::WavetableOsc());
    oscs[i]->sine(9);
    oscs[i]->freq(0);
    oscs[i] >> pan;

    envs.emplace_back(new Clover::NodeSimplex::Envelope::BasicEnvelope(
        distribution(gen), distribution(gen), SAMPLE_RATE * 10));
    envs[i] >> blackHole;
  }

  interface.clock.registerTickCallback([&](int currentTime) -> void {
    for (int i = 0; i < numOscs; i++) {
      oscs[i]->freq(envs[i]->frames.current[0]);
    }
  });

  if (interface.initialize() != paNoError)
    return 1;

  if (interface.openDevice(Pa_GetDefaultOutputDevice()) != paNoError)
    return 1;
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
