#pragma once

#include <functional>
#include <stdexcept>
#include <vector>

#include "Node.h"
#include "NullFrame.h"

namespace Clover::Graph {

/// Base class for Nodes that do not exchange data but still need to be
/// connected to the graph
class NullNode : public Node<NullFrame, NullFrame> {};

} // namespace Clover::Graph