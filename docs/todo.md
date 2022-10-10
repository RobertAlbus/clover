# TODO

- add a `ProcessingLatency()` method to Node (for processing latency, ex. FIR filters)
- this has been invalidated because I removed the FIR filter which was the source of latency. everything else is latency-free.
- this may become relevant again in the future if I need to do any look-ahead such as for certain envelope followers
- most/all nodes coule really use a SpecificNodeImplementationSettings class for presets load/save maybe

## Sink

- .wav output
  - should this be a node?
  - how can I render a whole track vs stems?
- interface

  - should this be a node?
  - can I select which outputs I actually use, not just the interface?
  - input!

- Project
  - artist
  - title
  - duration
- sink
  - playback
    - sample clock
    - duration
    - `start()`
    - sample rate, bit depth, channels, etc?
    - device name/index/whatever
    - outputs?
  - wav
    - sample clock
    - duration
    - `start()`
    - sample rate, bit depth, channels, etc
    - output file relative path
      - append timestamp?

## Nodes

- should have a reset method that inits all values, including the `lastComputedClockTime` for instances of programmatic rendering of EG. multiple hihat samples
- should use getters and setters for all props on nodes

- Wavetable Osc
  - should use the heap for the wavetable (in order to support arbitrary size. beneficial for noise)
  - Noises
    - brown noise
    - clicks, pops, etc
    - character-ful noises IE tape
- Wave Shapers / Distortions / transforms
  - cubic interpolation
  - bicubic interpolation for morphing
  - linear interpolation
  - bilinear interpolation for morphing
  - rectifier
- Dynamics control
  - VCA compressor/limiter
  - gate
- Sampler
  - can sample N samples from a graph with a clock
  - can load a wave file
  - crossfade loop
- Delay
  - diffusion
  - echo
  - ping pong or custom-feedback-insert
  - fractional delay line
    - should support N-tap with default at 1
- Filters
  - freq-based interface for SVF
  - cross-over filter split
- Spectral Domain
  - FFT library integration
  - time stretch
  - pitch shift
  - vocode
  - bucket brigade and other weird stuff that fucks with spectra
- Envelope
  - Interface class so they all have the same shape
    - keyOn, keyOff
    - triggerable vs keyable? triggerable for envs, keyable for instruments
  - L1 envelope follower
  - L1 N-Stage
  - L1 impulse
  - automation clip - don't worry about looping automation clips. composition-level only.
  - notes:
    - should accept input
    - output == input + envelope
    - should be arity-able where all channels of the envelope are the same - will be needed to handle multi-arity inputs
    - don't implement arity yet tho, just lock to 1
- Source
  - L1 MIDI Source
  - L2 MIDI Signal (L1 + scale + smoothing + maybe shaping)
  - Audio input
- Mixing / Routing
  - L3 Channel
  - L1 Bus I/O? (high-arity single-node connection)
  - L3 Mixer
- Instruments
  - L3 NxOsc
  - L3 FMN (FM8ish) or DXN (Dx7ish)
  - Kicker
  - HH808
  - Clapper
  - Sner
- UI output
  - specral analyzer
  - metering (peak, RMS, etc)
  - waveform
  - composition/pattern displays?

https://github.com/thestk/stk
http://svr-www.eng.cam.ac.uk/comp.speech/Section1/AudioSoftware/spkit.html

## House Keeping

- build system

  - can be built as a cmake library

- namespacing
  - Clover
    - Graph (frame, history, node, stateless, subgraph, root node)
    - IO (audio IO, MIDI IO, OSC IO, .wav render)
    - NodeSimplex
      - basic
      - envelope
      - wavetable
      - waveshape
      - reduce
      - delay
      - stereo
      - midi
    - NodeComplex
      - high level FX and instruments...
      - spatial (echo, verb, convolver, etc)
      - dynamics (dynamics processors with character)
      - instrument (FMN, NxOsc, Kicker, HH808, etc)

## Ops

DO:

- clover git repo uses tags for versions (starting with v1 = first consumable version)
- clover git module added as submodule in consuming project
- clover CMakeLists can be targeted by consuming project's CMakeLists
- clover has submodules for portaudio, portmidi, and other cmake-able deps
  - https://github.com/CD3/libInterpolate

## Notes

Special thanks to
https://github.com/janne808/svf
for the referene material upon which my SVF is based.
