# TODO

- create a script for `rebuild.sh` that only nukes Clover, and not dependiencies
- migrate to RtAudio? or off of rtmidi? want to have the same provider, but RtMidi is buggy. or perhaps it is my controller.
- upgrade file names and folder names to use CapitalCaseNaming

## Wavetables
- Create noises
    - Gauss noise
    - clicks, pops, etc
    - character-ful noises IE tape
- fix click at the beginning of the WavetableOsc

## Node Base
- make every node have a ClassNameSettings object for all member variables
    - ctor should accept this (with a copy, not a ref)
    - fix the LERP direction when freqencies are negative
      - create "lerpNegative()" and "lerpPositive()" methods. 
      - when setting the freq, assign the correct function to "lerp()"
      - can optimize the sign lookup at the top of `tick()` on freq-assign as well
- improve code efficiency
  - create benchmarking function that sets up a graph and manually ticks it for a certain quantity of data
  - remove extraneous copies
  - convert all data in the node ingress to const reference args and returns

## Misc Nodes
- NodeSimplex::Limits
  - Min
  - Max
  - Clamp
  - RangeAvoider
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
- high-order construct for nicer MIDI input

## Audio IO
- create a better UI and paradigm for the Interface
- remove black hole nodes in the interface in favour of using null adapters
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
- moog ladder
- EQ

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
- http://rampmeupscotty.herokuapp.com/blog/2016/01/19/anatomy-of-midi-message/

- https://en.wikipedia.org/wiki/FFTW
- https://github.com/project-gemmi/benchmarking-fft

UI
- https://stackoverflow.com/questions/1283202/lightweight-c-gui-library

- https://github.com/Immediate-Mode-UI/Nuklear  (simple, clean, flat) <=== this one!
  - only one I could get working
  - no external dependencies
  - ANSI C

- http://cegui.org.uk/content/getting-started (games and cool stuff)
- https://www.fltk.org/ (application UIs)
- http://www.gnuplot.info (no just for plotting, interacts with UI lib)

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