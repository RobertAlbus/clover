- midi-in device
  - has long lived char[] array for midi state
    - one for notes, one for control change (144 vs 176)
  - has a long-lived bool[] for noteTrigger state, only true for the sample on which a note is activated
  - tick
    - arity<0,256>
    - EVENTUALLY could do <256,256> to combine midi signals maybe?
    - does it need a tick?
      - can just reference the state in the main callback
      - but then I can't signal-process midi
      - might wanna be able to do that, ie midi delays, echos, or other transforms
  - MAYBE:
  - spawns a thread
    - read midi messages
    - via midi state array pointer, writes messages
  - OR BETTER:
  - write a callback to pass to MidiIn to update the state