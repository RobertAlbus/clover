# ADR 02: Testing Priorities in Algorithms vs. Nodes

## Status

Accepted

## Context

Clover has a test suite focused on code correctness. Resulting from ADR-01, there are Graph Node classes that proxy to Algorithm classes. It is important to verify correctness of the algorithms and to verify the correctness of Nodes. This ADR outlines how to approach testing each of these classes in a way that provides a tangible increase in code confidence while not increasing test redundancy.

## Decision
Algorithm tests should focus on correctness and should be exhaustive.
- Default and parameterized initialization
- Getters and setters
- Results of `process()` including mutation of state

Graph Nodes should employ smoke tests. These tests merely verify that some useful portion of the interface is integrated to the underlying Algorithm.

### Example
The Mono Wavetable Oscillator _Algorithm_ is constructed from a Wavetable and a Wavetable Index Calculator. The Oscillator, the Wavetable, and the Calculator all individually covered by comprehensive tests.

The Mono Wavetable Oscillator _Node_ proxies to the relevant _Algorithm_ without implementing any custom logic. The tests for this Node only check that the Oscillator Node has the expected RMS level for some duration of a sine wave.

## Consequences

Benefits
- Clarity in testing priority
- Eliminates test duplication that presents minimal benefit

Drawbacks
- Node smoke tests may not be comprehensive enough to determine if a portion of the interface is not correctly integrated to the underlying Algorithm
