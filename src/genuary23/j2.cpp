#define NUM_SECONDS (100)

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

  int numOscs = 6;
  std::vector<Clover::NodeSimplex::Wavetable::WavetableOsc *> oscs;
  Clover::NodeSimplex::Stereo::Pan1 pan;
  pan.gain(0.3);
  pan >> interface.rootNode;

  //clang-format off
  using namespace Clover::Midi;
  std::vector<float> notes {
    Note::E4,
    Note::Fs4,
    Note::Gs4,
    Note::B4,
    Note::Cs5,
    Note::D5
  };
  //clang-format on

  for (int i = 0; i < numOscs; i++) {
    oscs.emplace_back(new Clover::NodeSimplex::Wavetable::WavetableOsc());
    oscs[i]->sine(9);
    oscs[i]->freq(Clover::Util::Calc::mtof(notes[i]));
    oscs[i] >> pan;
  }

  interface.clock.registerTickCallback([&](int currentTime) -> void {

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
