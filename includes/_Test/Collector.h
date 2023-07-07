#pragma once
#include <string>

#include "Graph.h"

namespace Clover::_Test {

template <size_t __arity>
class Collector : public Graph::AudioNode<__arity, __arity> {
public:
  Collector(int preAllocateFrames) : Graph::AudioNode<__arity, __arity>() {
    frames.reserve(preAllocateFrames);
  }

  std::vector<Graph::AudioFrame<__arity>> frames;

  void showBetween(size_t fromIndex, size_t toIndex) {
    // add this to the collector node to dump the buffer
    printf("\n\n");
    for (int i = fromIndex; i <= toIndex; i++) {
      std::string output = "\n ";
      output += std::to_string(i);
      for (int j = 0; j < __arity; j++) {
        output += " - ";
        output += std::to_string(frames[i][j]);
      }
      printf(output.c_str());
    }
    printf("\n\n");
  }

private:
  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    frames.emplace_back(input);

    return input;
  }
};

} // namespace Clover::_Test