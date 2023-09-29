# ADR 05: Scripts Are Make Targets

2023-09-13

## Status

Accepted

## Context

This project uses CMake to generate build files. These build files can compile the project as well as run scripts. Clover has, so far, used this only for compilation. The scripts for testing, code formatting, and so forth have all existsed as shell scripts.

## Decision

- One shell script is permitted, a `configure` script, which runs the CMake generator.
- Existing shell scripts should be converted into Makefile targets
- Specify script targets in a separate CMake file that is included in the main CMake file
- Leverage CMake utilities to ensure required binaries are present

## Consequences

Benefits
- Create a unified scripting approach
- CMake utilities are available
  - ensure the binaries required by the script are available
  - set the script context, such as the repo root folder or another path
- A stepping stone toward future CI/CD automation goals

Drawbacks
- Potentially higher learning curve for contributors to understand or contribute scripts

Neutral Changes
- Developers run scripts from the `build` directory instead of the root directory
