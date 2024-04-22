#pragma once

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

#include <string>

#include "Clover/Graph/AudioFrame.h"
#include "Clover/Graph/AudioNode.h"
#include "Clover/Graph/NullFrame.h"

namespace Clover::_Test {

template <size_t __arity> class Collector : public Graph::AudioInNode<__arity> {
public:
  Collector(int preAllocateFrames)
      : Collector(static_cast<size_t>(preAllocateFrames)) {}

  Collector(size_t preAllocateFrames) : Graph::AudioInNode<__arity>() {
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
  Graph::NullFrame tick(Graph::AudioFrame<__arity> input) {
    frames.emplace_back(input);

    return Graph::NullFrame{};
  }
};

} // namespace Clover::_Test
