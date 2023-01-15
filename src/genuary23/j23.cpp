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
  printf("\nClover Version %d.%d\n", Clover_VERSION_MAJOR,
         Clover_VERSION_MINOR);
  printf("\nDefault Audio Device Index: %d\n", Pa_GetDefaultOutputDevice());

  Interface interface;
  Time time(160, SAMPLE_RATE, &interface.clock);
  Clover::NodeSimplex::Adapter::NullAdapter<0, 2> blackHole;
  blackHole >> interface.rootNode;

  std::string wavPath = "/home/trax/code/music/clover/renders/j23.wav";
  Clover::IO::WavFile<2> wavOut(wavPath, 480000);
  wavOut >> blackHole;

  int numOscs = 4;
  std::vector<Wavetable::WavetableOsc *> oscs{};
  std::vector<float> detune{-3., -2., 2., 3.};
  float spreadMultiplier = 0.;
  float baseNote = Clover::Midi::Note::F1;

  float baseFiltFreq = 500.;
  Clover::NodeSimplex::Filter::BiQuad<2> filter;
  filter.lowPass();
  filter.set(baseFiltFreq, 0.9);
  filter >> interface.rootNode;
  filter >> wavOut;

  Clover::NodeSimplex::Stereo::Pan1 panL(-0.2);
  Clover::NodeSimplex::Stereo::Pan1 panR(0.2);
  panL >> filter;
  panR >> filter;

  Clover::NodeSimplex::Wavetable::WavetableOsc lfo;
  lfo.sine(9.);
  lfo.freq(.1);
  lfo.phase(0.75);
  lfo >> *(new Clover::NodeSimplex::Adapter::NullAdapter<1, 0>()) >> blackHole;

  filter.gain(0.5);

  for (int i = 0; i < numOscs; i++) {
    Wavetable::WavetableOsc *osc = new Wavetable::WavetableOsc();
    oscs.emplace_back(osc);
    osc->saw(2222);

    if (i % 2 == 0) {
      oscs[i] >> panL;
    } else {
      oscs[i] >> panR;
    }
  }

  interface.clock.registerTickCallback([&](int currentTime) -> void {
    float lfoNormalized = (1. + lfo.frames.current[0]) / 2.;
    filter.freq(baseFiltFreq + ((1 - lfoNormalized) * 2000.));

    spreadMultiplier = lfoNormalized * 20;

    for (int i = 0; i < numOscs; i++) {
      float midiNote = baseNote + (detune[i] * 0.01 * (spreadMultiplier * 0.5 + 0.5));
      oscs[i]->freq(Clover::Util::Calc::mtof(midiNote));
    }
  });

  // this can live in the interface.
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

  // interface.playFor(time); ??????
  std::this_thread::sleep_for(std::chrono::seconds(NUM_SECONDS));
  interface.stop();

  return paNoError;
}
