#pragma once

#include <algorithm>
#include <string>

#include "wave/file.h"

#include "Graph.h"
#include "Util/Calc.h"

using namespace Clover::Util;

namespace Clover::IO {

template <size_t __arity> class WavFile : public Graph::AudioNode<__arity, 0> {
public:
  WavFile(std::string filePath, int duration)
      : Graph::AudioNode<__arity, 0>(), _durationSamples(duration), _filePath(filePath)

  {
    content.reserve((size_t)duration * __arity);
    int result = writeFile.Open(filePath, wave::kOut);

    writeFile.set_channel_number((uint16_t)__arity);
    writeFile.set_sample_rate(48000);  // add sample rate to base class Clover
    writeFile.set_bits_per_sample(32); // add bit depth
  }

  int duration() { return _durationSamples; }

private:
  // WavFile() : Node() {}
  wave::File writeFile;
  std::vector<float> content;

  int _durationSamples;
  std::string _filePath;
  float _signalMaxima;

  Graph::AudioFrame<0> tick(Graph::AudioFrame<__arity> input) {
    Graph::AudioFrame<0> f{};
    if (Graph::AudioNode<__arity, 0>::_currentClockTime == _durationSamples) {
      // TODO: make this configurable with options
      // - normalize if small
      // - normalize if large
      normalizeContent();
      bool clip = true;
      int result = writeFile.Write(content, clip);
      return f;
    }

    for (int i = 0, end = (int)__arity; i < end; i++) {
      _signalMaxima = std::max(_signalMaxima, fabs(input[i]));
      content.emplace_back(input[i]);
    }

    return f;
  }

  void normalizeContent() {
    float minusThreeDb = Calc::dbtol(-3.);
    for (int i = 0, end = content.size(); i < end; i++) {
      content[i] = content[i] / _signalMaxima * minusThreeDb;
    }
  }
};

} // namespace Clover::IO