# ADR 01: Node As Proxy

## Status

Accepted

## Context

Clover exposes base classes for creating Nodes in a Directed Graph. Since inception of the library, algorithms have been implemented as subclasses of the Graph Node. These algorithms are also useful outside of the graph-based context. Perhaps multiple algorithms might be useful in conjunction with each other, but the overhead of setting up a graph might be excessive.

## Decision

- Algorithms should be implemented in non-graph classes.
- Algorithms should extend `AlgorithmBase`
  - Algorithms must expose a `process()` method
  - The `process()` method must return a value 
  - This ADR does not distate any requirements about arguemnts to the `process()` method  
  - The algorithm result must be assigned to `this->processed` before returning from `process()`
- Graph Nodes should only proxy to Algorithm classes and not contain any algorithm logic themselves

## Consequences

Benefits
- Algorithms can be used in non-graph contexts
- Multiple algorithms can be used in concert without needing to orchestrate a graph 

Drawbacks
- Adds indirection when looking into the algorithm/s behind a Graph Node
- Proxy-only approach to Graph Nodes adds boilerplate
