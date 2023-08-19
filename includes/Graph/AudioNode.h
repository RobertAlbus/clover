#pragma once

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