# Code Structure

* Graph
    * Frame
    * Node
    * Stateful Nodes
        * pan
        * delay
        * dc
        * envelope
        * ADSR
        * automation clip
        * svf
        * scaler
        * bit crusher
        * sample crusher
        * wavetable
            * waves
            * noises
        * diffuser
        * Mixer Channel
        * Mixer
    * Stateless Nodes
        * sum
        * difference
        * gain
        * peel
        * clip
        * inverter
        * Bus IO
        * universal distortion?? (lambda captures a global variable for distortion amount to create a macro-effect for several instances)
    * Subgraph Nodes
        * stereo imager (doesn't have to be... if it's inefficient should recreate as a Custom Node)
        * ADSR
        * echo
            * a delay with built in fb and fb insert
            * custom common echos: ping pong
        * reverb
        * Instruments
            * must be key-able with MIDI note
            * NxOsc
            * FMN
    * Side Effect Nodes
        * step sequencer
        * arrangement tools
* utilities
    * ring buffer
    * frame history
    * sample clock
    * music time
    * calc
    * Frame History