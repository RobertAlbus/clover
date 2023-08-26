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


#include <cmath>

#include "Graph.h"

namespace Clover::NodeSimplex::Waveshape {

template <size_t __arity>
class RingMod_V1 : public Graph::AudioNode<__arity, __arity> {
public:
  RingMod_V1() : Graph::AudioNode<__arity, __arity>(), _shape(0) {}

  void shape(float s) { _shape = s; }
  float shape() { return _shape; }

private:
  float _shape;
  Graph::AudioFrame<__arity> tick(Graph::AudioFrame<__arity> input) {
    float shape = ((_shape * 0.5) + 0.5) * M_PI;
    Graph::AudioFrame<__arity> f{};
    for (size_t i = 0; i < __arity; i++) {
      f[i] = sin(shape * input[i]);
    }

    return f;
  }
};

} // namespace Clover::NodeSimplex::Waveshape
