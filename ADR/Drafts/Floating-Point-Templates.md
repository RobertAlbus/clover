# Draft: Floating Point Concepts

## Status

Draft

## Context

Algorithms usually operate on floating point numbers. In many cases a plain `float` may offer enough precision for the task at hand. Higher precision by way of `double` or `long double` may be required in other cases for various reasons. This ADR presents a decision on how to prevent duplicating code for each floating point type, yet still allowing the use of lower-precision types when suitable.

TODO: test the speed efficiency of `long double` vs. `float` operations to determine if this presents any practical benefit

## Decision
- All Algorithm classes must be type generic, using a C++20 Concept as a template parameter 
- PENDING: should the template param have an enforced name, such as `T` or `Floating`?

## Consequences

Benefits
- Reduce duplication: create just one implementation for all floating point types
- POTENTIAL: faster execution when less precision is acceptable, only paying for higher precision when needed

Drawbacks
- Use of templates may increase the knowledge and skill barriers to contribute to this project
