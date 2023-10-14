# Benchmarking & Performance Testing

## Profilers
- a suite of classes and/or functions that that execute standard units of benchmarking work

## Profiles
- Encapsulation of expected performance for a given environment
- main dev machine, laptop, standardized CI server, etc.

## BenchmarkProfileSerializers
- read and write BenchmarkProfile to YAML/json/toml/sqlite/whatever

## Performance Tests
- GTest that asserts Profiler results against Profile expectations