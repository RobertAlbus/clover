# TODO

once SVF is created, all nodes should have a `latency()` method that returns the samples of latency. it would be great to have this calculable between two nodes, but that is a dream for now.
-- this has been invalidated because I removed the FIR filter which was the source of latency. everything else is latency-free.
-- this may become relevant again in the future if I need to do any look-ahead such as for certain envelope followers

## Nodes

- should have input and output gain

- Wave Shapers / Distortions
  - cubic interpolation
  - bicubic interpolation for morphing
  - linear interpolation
  - bilinear interpolation for morphing
- Dynamics control
  - compressor/limiter
    - input gain
    - wave shaping function?
  - gate
- Sampler
  - can sample N samples from a graph with a clock
  - can load a wave file
  - crossfade loop
- Delay
  - echo
  - ping pong or custom-feedback-insert
  - upgrade buffer
    - make it a fractional delay line
    - make it a Node
- ~~ Filters ~~
  - ~~ SVF with oversampling ~~
    - SVF with freq-based interface
    - do I really need the oversampling and downsampling FIR? musically, not strictly necessary.
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
  - L1 N-Stage
  - envelopes should handle input
  - all envelopes should add the envelope value to the input value
  - envelopes should be arity-able where all channels of the envelope are the same - will be needed to handle multi-arity inputs
  - don't implement arity yet tho, just lock to 1
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
- UI output
  - specral analyzer
  - metering (peak, RMS, etc)
  - waveform
  - composition/pattern displays?

## House Keeping

- build system

  - aggregation headers for simpler imports FAKE AND DUMB I GUESS?
  - can be imported into other projects as a git submodule
  - can be built as a cmake library
  - convert non-template header implementations to source implementations (not now, why bother with the effort until build are too slow)
  - deps should be included via cmakelists instead of the system (yeah?)

- namespacing
  - Clover
    - Graph (frame, history, node, stateless, subgraph, root node)
    - IO (audio IO, MIDI IO, OSC IO, .wav render)
    - NodeSimplex
      - basic (gain, invert, rectifier, stateless processor and subgraph base classes)
        - rectifier
        - multiplier (just use a gain)
      - envelope (standard, ADSR, follower, N-stage, DC, impulse, etc)
      - wavetable
      - waveshape (distortions)
      - reduce (bit/sample crushers)
      - delay (fractional delay line)
      - stereo (sum, difference, pan1/2, peel)
      - midi
    - NodeComplex
      - spatial (echo, verb, convolver, etc)
      - dynamics
      - instrument (FMN, NxOsc)

## Ops

how do I make Clover into a package? Is it necessary? Don't do artifacts, do git submodules, version tags, and cmakelists

DO:

- clover git repo uses tags for versions (starting with v1 = first consumable version)
- clover git module added as submodule in consuming project
- clover CMakeLists can be targeted by consuming project's CMakeLists
- clover has submodules for portaudio, portmidi, and other cmake-able deps
  - https://github.com/CD3/libInterpolate

NOT:

- Conan C++ - package manager?
- JFrog Artifactory Community Edition? - package repo
- or packages from repos via cmake? <-- probably>

https://conan.io/
