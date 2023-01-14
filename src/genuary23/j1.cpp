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
  Clover::NodeSimplex::Adapter::NullAdapter<1, 2> blackHole;
  blackHole >> interface.rootNode;

  int numOscs = 6;
  std::vector<Wavetable::WavetableOsc *> oscs{};
  // clang-format off
  std::vector<float> freqs {222, 0.3, 0.305, 0.61, 0.92, 1.83, .54, 0.888, 7, 11};
  for (int i = 1; i < freqs.size(); i++) {
    freqs[i] = freqs[i] / 1000.;
  }
  std::vector<float> ranges{244, 99,  300,   20,   19,   64,   54,  888, 7, 11};
  // clang-format on
  float baseFreq = 300;


  for (int i = 0; i < numOscs; i++) {
    Wavetable::WavetableOsc *osc = new Wavetable::WavetableOsc();
    oscs.emplace_back(osc);
    if (i == 0) {
      osc->sine(66622);
    } else {
      osc->sine(2222);
    }

    // float modRange = baseModRange * pow(2,i);
    // ranges.emplace_back(modRange*(i+4.96*3.97));
    // freqs.emplace_back(baseFreq*(i*i/(i-3.44)));
  }

  Clover::NodeSimplex::Filter::BiQuad<2> filter;
  filter.lowPass();
  filter >> interface.rootNode;

  for (int i = 0; i < numOscs; i++) {
    oscs[i]->freq(baseFreq);

    if (i == 0) {
      oscs[i] >> *(new Clover::NodeSimplex::Stereo::Pan1) >> filter;
    } else {
      oscs[i] >> blackHole;
    }
  }

  Clover::NodeSimplex::Range::Avoid<1> fbDestroyer(0., 0.);
  oscs[0] >> fbDestroyer >> blackHole;

  interface.clock.registerTickCallback([&](int currentTime) -> void {
    for (int i = 0; i < numOscs - 1; i++) {
      float modAmount =
          (ranges[i] * oscs[i + 1]->frames.current[0]) / 5.f * 7.f;
      oscs[i]->freq(freqs[i] + modAmount);
    }
    float fbMod = fbDestroyer.frames.current[0];
    filter.set( 2222 * ((fbMod + 1)/2) , 0.8);

    // float existingFreq = oscs[numOscs-1]->freq();
    // oscs[numOscs-1]->freq(existingFreq * fbMod);
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
  std::this_thread::sleep_for(std::chrono::seconds(NUM_SECONDS));

  interface.stop();
  return paNoError;
}
