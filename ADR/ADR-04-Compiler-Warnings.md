# ADR 04: Compiler Warnings

## Status

Accepted

## Context

Compilers can provide valuable insight to developers in the form of warnings. These warnings may be configured to report on outdated approaches, undefined behaviour, common bugs, etc. These warnings may be explicitly included or ignored. They may additionally be treated as errors, which fails the compilation.

## Decision

A comprehensive suite of compiler warning flags will be added to the build in CMakeLists. The Clover library target will be duplicated with these warnings. This target should additionally have the `-Werror` flag to treat warnings as errors, failing the compilation. This target will be excluded from the `all` group, so it must be explicitly built.

## Consequences

### Benefits:
- Increase awareness surrounding code practices that lead to warnings
- Provide a build target that can be used in CI for validating code quality

### Drawbacks:
- Noisier build outputs
- Does not provide actual enforcement of code quality