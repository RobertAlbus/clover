# Inter Process Communication

- Adding IPC may induce the need for thread-safe logging for the purposes of debugging
- consider the tradeoffs between ring buffers and swappable buffers
- this graph will not require explicit cycle interuption
  - old version of graph, Clover::Graph::Node, the graph was walked by DFS
  - new version of graph does not need a root node, a clock time, etc
    - a clock time may still be needed for time-based tools envelopes and sequencers
    - so it should be tracked and passed in
    - tracking the clock internal in a specific Node could result in de-sync when realtime performance temporarily drops.

## "Node"
- AudioProcess<InType, OutType> class
  - spawns a thread for (AudioProcess::run, this)
    - lock-free, wait free signaling
    - use std::atomic for signal
    - bool hasInput
    - bool hasOutput

## "Graph"
- AudioGraph class
  - know's it's per-tick time budget, and continues even if a node is not complete
  - all edges are stored in an adjacency matrix
    - consider dynamically building lambdas for summing data
      - Node A has input from Nodes B, C
      - create a lambda that sums B, C before passing to A
    - has the trade-off that the graph cannot be dynamically updated 
  - Graph.ConnectNodes(NodeA, NodeB, weight);
  - consider sorting the graph:
    - https://chat.openai.com/c/0f305e8b-c4a5-4fdb-90e7-e12f49d95c47



