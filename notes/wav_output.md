# Wav File Output

Consider changing over to this library:
https://github.com/kleydon/WavUtils

because it can perform incremental reading/writing of wav files.

## Possible Approaches

- Node based
  - is itself a Node
  - is given frame count, multiplied by with Node Arity is the total size
  - connect a Node to the WavNode, WavNode to black hole
- Node-consumer based
  - receives a Node ref
  - manually ticks the root node with an owned instance of a sample clock
  - I don't think this works well!
  - What about multi-stem renderings where multiple non-root nodes are used for wav file material

## Node Based Ideation

- connect any node to a Wav2
- do I want to provide a stop time?
- would I be better suited to a recursive Finish() callback on Node
  - can be used for general cleanup/reset
  - more Node enhancements in general could be good.
  - tick(), nodeTick()
  - finish(), nodeFinish() ie. finish wav file and flush to disk.
  - reset(), nodeReset() ie. reset clock
  - might also need a state-tracker
    - if a node is connected to more than one sink directly or indirectly, would want to skip the reset
    - should also therefore create an WavetableState preset class (for example) so that a node can be reset to it's initial state/preset on reset
    - store a CurrentState and an InitState instance of this on the Node
