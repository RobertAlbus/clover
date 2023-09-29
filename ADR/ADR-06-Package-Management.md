# ADR 05: Package Management

2023-09-29

## Status

Accepted

## Context

This project has external dependencies. These dependencies have been managed with either CMake fetch_content or git submodules. Using multiple methods of managing dependencies is less than ideal. Both approaches also require significant configuration to set up. 

## Decision

- Dependencies will come from the vcpkg package manager
- vcpkg will be used in manifest mode
- vcpkg will be explicitly integrated directly in CMakeLists.txt

## Consequences

Benefits
- Easier adoption and deprecation of dependencies with a declarative manifest file
- Eliminates the need to configure the builds for dependencies
- Automatic cross platform support via vcpkg for compatible dependcies
- Faster builds due to vcpkg caching

Drawbacks
- Introduces a new tooling dependency to the project
  - It is possible and trivial to remove the direct CMake integration of vcpkg, allowing for per-developer toolchain configuration. This minimizes the impact of the tooling dependency.