#pragma once

#include <vector>
#include <cassert>

#include "RtMidi.h"

#include "node.h"

using namespace Clover::Graph;

namespace Clover::IO {

class MidiIn : public Node<0,256>
{
public:
  MidiIn(
    const std::string& deviceName,
    unsigned int queueSizeLimit = 1000 
  ) : Node(), _printChange(false)
  {
    RtMidi::Api api = RtMidi::Api::UNSPECIFIED;
    const std::string& clientName = "Clover Midi Input Client";
    controlChannel.resize(128);
    noteChannel.resize(128);

    try {
      rtMidi = new RtMidiIn(api, clientName, queueSizeLimit);

      // Don't ignore sysex, timing, or active sensing messages.
      rtMidi->ignoreTypes( false, false, false );
      rtMidi->setCallback(rtMidiCallback, this);

      rtMidi->openPort(getPortNumberByName(deviceName));

    }
    catch ( RtMidiError &error ) {
        error.printMessage();
        exit( EXIT_FAILURE );
    }


  }
  ~MidiIn()
  {
    rtMidi->closePort();
    delete rtMidi;
  }

  static void printPorts()
  {
    RtMidiIn rtMidiIn;
    unsigned int nPorts = rtMidiIn.getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ ) {
      try
      {
        portName = rtMidiIn.getPortName(i);
      }
      catch ( RtMidiError &error )
      {
        error.printMessage();
      }
      std::cout << "  " << i << ": " << portName << '\n';
    }
  }

  void printChange(bool shouldPrint) { _printChange = shouldPrint; }
  bool printChange()                 { return _printChange;        }

  int getPortNumberByName(const std::string& selectedDeveiceName)
  {
    unsigned int nPorts = rtMidi->getPortCount();
    std::string portName;
    for ( unsigned int i=0; i<nPorts; i++ ) {
      try
      {
        if (selectedDeveiceName == rtMidi->getPortName(i)) return i;
      }
      catch ( RtMidiError &error )
      {
        error.printMessage();
      }
    }
    exit(EXIT_FAILURE);
  }

private:
  bool _printChange;
  std::vector<unsigned char> noteChannel;
  std::vector<unsigned char> controlChannel;
  RtMidiIn* rtMidi = 0;

  static void rtMidiCallback( double timeStamp, std::vector<unsigned char> *message, void * midiInRef ){
    assert(midiInRef != NULL);
    MidiIn* midiIn = (MidiIn*)midiInRef;

    std::vector<unsigned char>* midiRegister;

    if (message->at(0) == (unsigned char) 0x90 /*note*/)
    {
      midiRegister = &(midiIn->noteChannel);
    }
    else if (message->at(0) == (unsigned char) 0xB0 /*control*/)
    {
      midiRegister = &(midiIn->controlChannel);
    }
    else
    {
      printf("\nClover received a MIDI message of type %c, which is unsupported", message->at(0));
      exit(EXIT_FAILURE);
    }

    if(midiIn->printChange())
    {
      printf("\n%i | %i | %i", message->at(0), message->at(1), message->at(2));
    }
    midiRegister->at(message->at(1)) = message->at(2);
  }

  Frame<256> tick(Frame<0> input) 
  {
    Frame<256> f;

    for ( int i = 0; i < 128; i++)
    {
      f[i]     = noteChannel[i];
      f[i+127] = controlChannel[i];
    }

    return f;
  }
};

}