#pragma once

/*
 * /////////
 * // Clover
 * 
 * Audio processing algorithms and DAG with feedback loops that do not break
 * acyclicity.
 * 
 * Copyright (C) 2023 Rob W. Albus
 *  
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 */


#include <assert.h>
#include <vector>

#include "RtMidi.h"

#include "Graph/AudioNode.h"

namespace Clover::IO {

class MidiIn : public Graph::AudioNode<0, 256> {
public:
  MidiIn(const std::string &deviceName, unsigned int queueSizeLimit = 1000)
      : Graph::AudioNode<0, 256>(), _printChange(false) {
    RtMidi::Api api = RtMidi::Api::UNSPECIFIED;
    const std::string &clientName = "Clover Midi Input Client";
    controlChannel.resize(128);
    noteChannel.resize(128);

    try {
      rtMidi = new RtMidiIn(api, clientName, queueSizeLimit);

      // Don't ignore sysex, timing, or active sensing messages.
      rtMidi->ignoreTypes(false, false, false);
      rtMidi->setCallback(rtMidiCallback, this);

      rtMidi->openPort(getPortNumberByName(deviceName));

    } catch (RtMidiError &error) {
      error.printMessage();
      exit(EXIT_FAILURE);
    }
  }
  ~MidiIn() {
    rtMidi->closePort();
    delete rtMidi;
  }

  static void printPorts() {
    RtMidiIn rtMidiIn;
    unsigned int nPorts = rtMidiIn.getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
    std::string portName;
    for (unsigned int i = 0; i < nPorts; i++) {
      try {
        portName = rtMidiIn.getPortName(i);
      } catch (RtMidiError &error) {
        error.printMessage();
      }
      std::cout << "  " << i << ": " << portName << '\n';
    }
  }

  void printChange(bool shouldPrint) { _printChange = shouldPrint; }
  bool printChange() { return _printChange; }

  int getPortNumberByName(const std::string &selectedDeveiceName) {
    unsigned int nPorts = rtMidi->getPortCount();
    std::string portName;
    for (unsigned int i = 0; i < nPorts; i++) {
      try {
        if (selectedDeveiceName == rtMidi->getPortName(i))
          return i;
      } catch (RtMidiError &error) {
        error.printMessage();
      }
    }
    exit(EXIT_FAILURE);
  }

private:
  bool _printChange;
  std::vector<unsigned char> noteChannel;
  std::vector<unsigned char> controlChannel;
  RtMidiIn *rtMidi = 0;

  static void rtMidiCallback(double timeStamp,
                             std::vector<unsigned char> *message,
                             void *midiInRef) {
    assert(midiInRef != NULL);
    MidiIn *midiIn = (MidiIn *)midiInRef;

    std::vector<unsigned char> *midiRegister;

    if (message->at(0) == (unsigned char)0x90 /*note*/) {
      midiRegister = &(midiIn->noteChannel);
    } else if (message->at(0) == (unsigned char)0xB0 /*control*/) {
      midiRegister = &(midiIn->controlChannel);
    } else {
      printf(
          "\nClover received a MIDI message of type %c, which is unsupported",
          message->at(0));
      exit(EXIT_FAILURE);
    }

    if (midiIn->printChange()) {
      printf("\n%i | %i | %i", message->at(0), message->at(1), message->at(2));
    }
    midiRegister->at(message->at(1)) = message->at(2);
  }

  Graph::AudioFrame<256> tick(Graph::AudioFrame<0> input) {
    Graph::AudioFrame<256> f;

    for (int i = 0; i < 128; i++) {
      f[i] = noteChannel[i];
      f[i + 128] = controlChannel[i];
    }

    return f;
  }
};

} // namespace Clover::IO
