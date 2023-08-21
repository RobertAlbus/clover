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


#include <functional>
#include <stdexcept>
#include <vector>

#include "AudioFrame.h"
#include "Node.h"
#include "NullFrame.h"

namespace Clover::Graph {

/// Base class for all N channel nodes of the audio graph.
template <size_t __arityInput, size_t __arityOutput>
class AudioNode
    : public Node<AudioFrame<__arityInput>, AudioFrame<__arityOutput>> {};

template <size_t __arity>
class AudioInNode : public Node<AudioFrame<__arity>, NullFrame> {};

template <size_t __arity>
class AudioOutNode : public Node<NullFrame, AudioFrame<__arity>> {};

} // namespace Clover::Graph
