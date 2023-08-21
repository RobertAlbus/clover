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
#include <cmath>
#include <functional>

#include "Algorithm.h"
#include "Base.h"
#include "Graph.h"

namespace Clover::NodeSimplex::Filter {

class FilterSettable {
public:
  virtual void set(float f, float Q) = 0;

  virtual void freq(float f) = 0;
  virtual float freq() = 0;

  virtual void reso(float Q) = 0;
  virtual float reso() = 0;

  virtual void lowPass() = 0;

  virtual void highPass() = 0;

  virtual void notch() = 0;

  virtual void bandPass() = 0;
};

} // namespace Clover::NodeSimplex::Filter
