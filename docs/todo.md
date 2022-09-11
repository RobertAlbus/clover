# TODO

once SVF is created, all nodes should have a `latency()` method that returns the samples of latency. it would be great to have this calculable between two nodes, but that is a dream for now.

## Nodes


- Delay
  - echo
  - ping pong or custom-feedback-insert
- ~~ Filters ~~
  - ~~ SVF with oversampling ~~
    - SVF with freq-based interface
    - do I really need the oversampling and downsampling FIR?
- Spectral Domain
  - FFT library integration
  - time stretch
  - pitch shift
  - vocode
  - bucket brigade and other weird stuff that fucks with spectra
- ~~ Envelope ~~
  - ~~ L1 Envelope ~~
  - ~~ L2 ADSR ~~
  - L1 envelope follower 
- automation clip - don't worry about looping automation clips. composition-level only.
- Source
  - L1 MIDI Source
  - L2 MIDI Signal (L1 + scale + smoothing + maybe shaping)
- Mixing / Routing
  - L3 Channel
  - L1 Bus I/O? (high-arity single-node connection)
  - L3 Mixer
- .wav output
- diffusion
- brown noise
- L3 NxOsc
- L3 FMN (FM8ish) or DXN (Dx7ish)

## Ops
how do I make Clover into a package? Is it necessary?

- Conan C++ - package manager?
- JFrog Artifactory Community Edition? - package repo
- or packages from repos via cmake? <-- probably>

https://conan.io/
