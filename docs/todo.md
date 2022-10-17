# TODO

- update cmake to compile the library separately from `main.cpp`
- create a script for `rebuild.sh` that only nukes Clover, and not dependiencies

## Wavetables
- Make Wavetable Osc use the head instead of the stack for wave data
- Create noises
    - Gauss noise
    - clicks, pops, etc
    - character-ful noises IE tape
- add phase member (-1..1)

## Node Base
- add lifecycle hooks and callbacks
    - tick
    - reset
    - etc (define more when doing this task)
- make every node use getter and setter methods, with member properties private
- make every node have a ClassNameSettings object for all member variables
    - make re-init, presets, etc. much easier.
    - ctor should accept this (with a copy, not a ref)
- add a connectNode check to prevent duplicate entries!

## Misc Nodes
- rectifier
- stereo invert
- NodeSimplex::Limits
  - Min
  - Max
  - Clamp
  - inspired by the math needed to solve the "dead zone" issue when modulation reaches 0hz in FM feedback networks

## Envelopes
- create a base class for all envelopes
- upgrade existing envelopes to multi-arity
- upgrade existing envelopes to handle input
- Create new envelope types
    - Follower (with hold and decay)
    - N-Stage
    - Impulse
    - Automation Clip
- add BasicEnvelope::isComplete() or similar 

# Dynamics control
- VCA compressor/limiter
- gate

## MIDI
- raw MIDI input
- high-order construct for nicer MIDI input
- note utils

## Audio IO
- create a better UI and paradigm for the Interface
- add audio input

## Sampler
- can sample N samples from a graph with a clock
- can load a wave file
- crossfade loop

## Instruments
- L3 NxOsc
- L3 FMN (FM8ish) or DXN (Dx7ish)
- Kicker
- HH808
- Clapper
- Snare

## Mixing / Routing
- L3 Channel
- L1 Bus I/O? (high-arity single-node connection)
- L3 Mixer

# Filters
- multi-arity SVF
- freq-based interface for SVF
- cross-over filter split (arity<1,2>, arity<2,4>)

# Interpolation
- cubic interpolation
- bicubic interpolation for morphing
- linear interpolation
- bilinear interpolation for morphing

# Delay
- diffusion
- multi-tap fractional delay line


## reference material
- https://github.com/webprofusion/OpenAudio
- https://github.com/twoz/binaural-vst (head related transfer functions for mono=>stereo as JUCE VST)
- https://www.reddit.com/r/DSP/comments/x5kopp/fir_filter_for_downsampling/ (multirate filters, downsampling FIR)
- https://ccrma.stanford.edu/~jos/pasp/Fractional_Delay_Filtering_Linear.html

  spline:
- https://stackoverflow.com/questions/1204553/are-there-any-good-libraries-for-solving-cubic-splines-in-c
- https://github.com/NCAR/bspline
- https://github.com/ttk592/spline
- https://kluge.in-chemnitz.de/opensource/spline/

- https://signalsmith-audio.co.uk/contact/
- https://github.com/janne808/svf
- https://stackoverflow.com/questions/32889309/adding-gaussian-noise
- https://github.com/thestk/stk
- http://svr-www.eng.cam.ac.uk/comp.speech/Section1/AudioSoftware/spkit.html
- https://github.com/CD3/libInterpolate
- https://github.com/PortMidi/portmidi

- https://en.wikipedia.org/wiki/FFTW
- https://github.com/project-gemmi/benchmarking-fft

UI

- https://github.com/Immediate-Mode-UI/Nuklear

C++ style standards
- http://www.possibility.com/Cpp/CppCodingStandard.html#noabbrev





## RETHINKY THINK

- Engine
  - sucks samples
  - state tracked by a clock, this isn't a clock but it owns one
  - auto-clocker for offline rendering
  - 
- interface
  - should this be a node?
  - can I select which outputs I actually use, not just the interface?
  - input!

- Project
  - artist
  - title
  - duration

## UNPRIORITIZED
- Spectral Domain
  - FFT library integration
  - time stretch
  - pitch shift
  - vocode
  - bucket brigade and other weird stuff that fucks with spectra

- UI output
  - specral analyzer
  - metering (peak, RMS, etc)
  - waveform
  - composition/pattern displays?