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

MIDI:
- midi smoothing: https://www.kvraudio.com/forum/viewtopic.php?p=7955616&sid=c27fe476fe3f16c6a7c159f88038714a#p7955616
- https://midi.org/specifications
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
  - NO KNOBS :( )

- http://cegui.org.uk/content/getting-started (games and cool stuff)
- https://www.fltk.org/ (application UIs)
- http://www.gnuplot.info (no just for plotting, interacts with UI lib)

C++ style standards
- http://www.possibility.com/Cpp/CppCodingStandard.html#noabbrev

Compiler optimization flags:
- https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html



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