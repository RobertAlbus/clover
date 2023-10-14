# Midi IO

TODO: Look into OSC

- Create an abstract interface for IO::IMidiIn
- Create a default IO::MidiIn implementation
- callback lambda
  - handles midi input
  ``` c++
  struct MidiNoteDatum {
    bool hasMessage;
    int messageType;
    int messageChannel;
    int messageValue;
  };

  MidiCallback = 
    [mySynth, pitchBendTracker](const std::vector<MidiNoteDatum>& midiDataIn)
        -> std::vector<MidiNoteDatum>{
            for (const MidiNoteDatum datum : midiDataIn) {
              if (!datum.hasMessage) continue;
              
              // handle message
              if (datum.messageValue > 0 && datum.messageType == NOTE) {
                mySynth.noteOn(datum.messageChannel, datum.messageValue);
              }

              if (datum.messageValue == PITCH_MOD && datum.messageType == CC) {
                float pitchBend = static_cast<float>(datum.messageValue) / 127.f;
                pitchBendTracker.set(pitchBend);
              }
            }
        };


  ```

