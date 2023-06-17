#pragma once

#include <functional>
#include <stdexcept>
#include <vector>

#include "Frame.h"
#include "FrameHistory.h"
#include "Node.h"

namespace Clover::Graph {

/// Base class for all N channel nodes of the audio graph.
template <size_t __arityInput, size_t __arityOutput>
class AudioNode : public Node<Frame<__arityInput>, Frame<__arityOutput>> {

};

} // namespace Clover::Graph