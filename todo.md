# TODO

once SVF is created, all nodes should have a `latency()` method that returns the samples of latency. it would be great to have this calculable between two nodes, but that is a dream for now.

## Nodes

- ~~ Stereo Image ~~
  - ~~ L1 mono pan ~~
  - ~~ L1 stereo pan ~~
  - ~~ L1 stereo sum ~~
  - ~~ L1 stereo difference ~~
  - ~~ L2 stereo imager (combo of previous two) ~~
- Delay
  - ~~ ring buffer ~~
  - ~~ Delay node (all pass filter) ~~
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
- ~~ StatelessProcessor (only a tick function, no additional state) ~~
  - ~~ StatelessProcessor<waveshapeFn> ~~
    - ~~ L1 Inverter ~~
- L1 Bit Crusher
- L1 Sample Crusher (not quite a wave shaper, current imagined implementation would require a bit of state)
- ~~ Envelope ~~
  - ~~ L1 Envelope ~~
  - ~~ L2 ADSR ~~
- automation clip - don't worry about looping automation clips. composition-level only.
- Source
  - ~~ L1 Noises ~~
  - ~~ L1 Wavetable Osc ~~
  - ~~ L1 DC ~~
  - L1 MIDI Source
  - L2 MIDI Source (L1 + scale + shaping)
- Mixing / Routing
  - L3 Channel
  - L1 Bus I/O? (high-arity single-node connection)
  - ~~L1 leftPeel (one channel of stereo into mono)~~
  - ~~L1 rightPeel (one channel of stereo into mono)~~
  - L3 Mixer
- L3 NxOsc
- L3 FMN (FM8ish) or DXN (Dx7ish)

## Ops
how do I make Clover into a package? Is it necessary?

- Conan C++ - package manager
- JFrog Artifactory Community Edition - package repo
- or packages from repos via cmake? <-- probably>

https://conan.io/
