# ADR 04: Compiler Warnings

## Status

Accepted

## Context

Compilers can provide valuable insight to developers in the form of warnings. These warnings can be configured to be included or ignored. The warnings can also be treated as errors, which fails the compilation.

## Decision

A comprehensive suite of compiler warning flags will be added to the build in CMakeLists. These warnings should be enabled with -DENABLE_WARNINGS. -Werror should not be included in the build at this time.

## Consequences

### Benefits:
- Increase awareness surrounding code practices that lead to warnings

### Drawbacks:
- Noisier build outputs
- Does not provide actual enforcement of code quality