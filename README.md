# Clover

Clover is a real time graph-based signal processing framework. It was initially developed for audio but can be used for arbitrary types of discrete time signals.

This project is largely experimental and liable to change drastically. I recommend referring to specific commit hashes if you consume this library. Clover requires a compiler with C++20 support.

Clover is permissively licensed and depends on open source software. See [LICENSE](./LICENSE) and [THIRD_PARTY.md](./THIRD_PARTY.md).


## Graph

Clover implements a directed graph for signal routing. The graph is not inherently buffered; the base Frame [concept](https://en.cppreference.com/w/cpp/concepts) does not require that the data field is a buffer. There may be performance tradeoffs with an unbuffered approach, but it allows for complex and phase-sensitive modulations that might otherwise be hard to achieve. There is not any technical limitation to implementing buffered frames if you should choose to do so.

Clover currently processes the graph in a single thread. Efforts are underway to parallelize the graph processing algoritm without compromising the established guarantees from the single-threaded engine.

### Extensible

Abstract base:
```c++
template <FrameConcept InputType, FrameConcept OutputType>
class Node {
    OutputType tick(const InputType input) = 0;
};
```

Example implementation:
```c++
class AudioToRgb : public Node<AudioFrame<1>, RgbFrame> {
    float lightness;

    RgbFrame tick(const AudioFrame<1> input) {
        return RgbFrame(doMagicalDspThing(input));
    }

    void adjustLightness(float lightnessFactor) {...}
    RgbFrame doMagicalDspThing(const AudioFrame<1> input) {...}
}
```

### Type-Level IO Matching
Clover ensures safety when connecting nodes by employing type-level IO matching. This ensures that only compatible nodes can be connected.

```c++
class Oscillator : public Node<EmptyFame,     AudioFrame<1>> {...};
class PanMono    : public Node<AudioFrame<1>, AudioFrame<2>> {...};

template <int ChannelCount>
class AudioNode
        : public Node<AudioFrame<ChannelCount>, AudioFrame<ChannelCount>> {...};
class Reverb : public AudioNode<2> {...};

Oscillator osc; 
PanMono pan;
StereoReverb reverb;

// Valid: all IOs match
osc >> pan >> reverb >> audioOutput.rootNode;

// Invalid: Output AudioFrame<1> of Osc doesn't match input AudioFrame<2> of Reverb
osc >> reverb;
// - will not compile
// - intellisense shows angry squigs
```

### Feedback Circuits and Acyclicity
Graph cycles are broken using conditional evaluation and state substitution. Practically speaking, feedback cycles are made acyclic by using the Node's output value from the previous tick in time.

```c++
Interface audioOutput;
float bpm = 160;
int SAMPLE_RATE = 48000;
Time time(bpm, SAMPLE_RATE, &audioOutput.clock);

OscNx<3> synth;
synth.loadPatch(mySynthPatch);

Gain g1;
Filter filter;
FractionalDelayLine<2> delay(SAMPLE_RATE * 3);
Gain feedbackGain;

filter.bandPass();
filter.freq(1000);
filter.reso(0.68);

delay.delay(time.step() * 6);
feedbackGain.gain(Calc::dbtol(-10));

// this graph has a synth with a 3/4 beat dub style band passed echo 
synth >> g1 >> audioOutput.rootNode;
g1 >> filter >> delay >> feedbackGain >> g1;
```

### Non-Audio Nodes
It is possible to implement side effect focused nodes such as step sequencers. There is an abstract base step sequencer that is fairly easy to extend:
```c++
struct Rgb {
    uint R;
    uint G;
    uint B;
};

struct RgbCallable {
  virtual void setRgb();
};

void RbgFacilitator(
        const TriggerState &data,
        std::vector<RgbCallable *> &targets) {

    for (RgbCallable *target : targets)
        target->setRgb(data);
}

struct STSQ_Rgb
    : STSQ<Rgb, RgbCallable, RbgFacilitator> {};

MyRgbNode rgbNodeStage1;
MyRgbNode rgbNodeStage2;
STSQ_Rgb rgbStepSequencer;

rgbStepSequencer.targets.emplace_back(&rgbNodeStage1);
rgbStepSequencer.targets.emplace_back(&rgbNodeStage2);

rgbStepSequencer.addPattern(blueStrobe);
rgbStepSequencer.addPattern(purpleWave);
rgbStepSequencer.addPattern(redWhitePulse);

```

There are semi-baked step sequencers implemented in the project. These may be either further refined or removed in the future.
```c++
STSQ_Pitchable stsq_pitch;
stsq_pitch.targets.emplace_back(&synth);
stsq_pitch.addPattern(someMidiNotePattern);

STSQ_Triggerable stsq_trigger;
stsq_trigger.targets.emplace_back(&synth);
stsq_trigger.addPattern(someMelody);
```

## Sample Clock Callback
This callback can be used however you see fit. 

```c++
interface.clock.registerTickCallback([&](int currentTime) -> void {
    if (isNewQuarterNote()) {
        kick.note(someMidiNote);
        kick.triggerOn(); // key on ADSR etc
    } else if (hasSixteenthNoteElapsed()) {
        kick.triggerOff(); // key off ADSR etc
    }

    // crunch some operations every clock increment
    // without needing to implement a Node
});
```


## Building
Clover uses CMake for the builds and vcpkg for dependency management. Refer to `CMakeLists.txt` and [vcpkg documentation](https://github.com/microsoft/vcpkg#getting-started).
