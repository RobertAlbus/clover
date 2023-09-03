# ADR 03: Performance Testing

## Status

Accepted

## Context

Real-time audio systems must process digital audio at a speed faster than playback. While the exact performance requirements are yet to be defined, flexibility for multiple requirement sets based on profiles like CPU architectures is anticipated. A notable improvement of 80% in the Wavetable Oscillator's speed was achieved through prior profiling and optimization efforts.

## Decision

In response to performance concerns and in the interest of maintaining a high-quality real-time audio system, the following decision has been made:

- Develop a dedicated test suite to precisely measure the execution time of performance-critical components.
- Ensure that the execution time of these components remains within acceptable limits aligned with the specific profile.
- Use Google Test, consistent with the existing test suites already used for correctness tests. This decision should be re-addressed if a broader decision regarding testing libraries is made.
- Existing tests should be moved to `test/unit`
- Performance tests should be created under `test/performance`

## Consequences

### Benefits:

- **Assured Performance:** The proposed test suite guarantees that the speed of performance-critical components stays well within acceptable limits.
- **Swift Regression Detection:** Running the performance suite facilitates the prompt identification of any performance regressions, enabling timely corrective measures.
- **Pinpointed Issue Resolution:** Leveraging tools such as `git bisect` will enable the identification of precise changes responsible for performance regressions, streamlining troubleshooting.

### Drawbacks:

- **Development Commitment:** Developers will need to allocate time to craft additional code for the performance test suite.
- **Benchmark Complexity:** Establishing acceptable performance benchmark values, especially across varying profiles or architectures, may present challenges.
