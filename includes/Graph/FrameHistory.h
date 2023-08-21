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


#include <algorithm>

#include "FrameConcept.h"
#include "Graph/AudioFrame.h"

namespace Clover::Graph {

template <Frame T> class FrameHistory {
public:
  FrameHistory() : _hasNext(false) {}

  void push(T frame) {
    if (!_hasNext)
      _next = frame;
    _hasNext = false;
    // todo: benchmark the current implementation with std::move compared to
    // previous implementation with std::swap
    // this is probably way better
    last = std::move(current);
    current = std::move(_next);
  }

  void next(T frame) {
    _hasNext = true;
    _next = frame;
  }

  T current = {};
  T last = {};

private:
  T _next = {};
  bool _hasNext;
};

} // namespace Clover::Graph
