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


#include <array>
#include <cstdio>

#include "Constants.h"

namespace Clover::Graph {

struct NullFrame {

  NullFrame() {}
  NullFrame(std::initializer_list<Sample> init) {}

  NullFrame operator+(const Graph::NullFrame &b) const { return b; }

  NullFrame operator-(const Graph::NullFrame &b) const { return b; }

  Graph::NullFrame operator*(const Graph::NullFrame &b) const { return b; }

  Graph::NullFrame operator/(const Graph::NullFrame &b) const { return b; }

  void operator+=(const Graph::NullFrame &b) {}

  void operator-=(const Graph::NullFrame &b) {}

  void operator*=(const Graph::NullFrame &b) {}

  void operator/=(const Graph::NullFrame &b) {}

  void operator+=(const float b) {}

  void operator-=(const float b) {}

  void operator*=(const float b) {}

  void operator/=(const float b) {}

  void init() {}
};

} // namespace Clover::Graph
