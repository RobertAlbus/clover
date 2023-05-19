# NxOsc


|-----------------------------------------|
|     stereo osc0    |       ..oscN       |-------------------|--------       
| tuning             |                    |  stereo osc LFOs  | pitch          
| pitch mod          |                    |-------------------| stereo detune      
| stereo detune      |                    |       ADSRs       | pan            
| osc pan            |                    |-------------------|                
|                    |                    |        
|-----------------------------------------|
|     dual filter (2 mono filters)        |-------------------|--------
| cutoff(freq) |> store as a member,      |  stereo osc LFOs  | cutoff  
| Q(q)         |                          |-------------------| Q       
| set(f,Q)     |                          |       ADSRs       |         
| modCutoff                               |-------------------|          
| modQ                                    |
|                                         |
|-----------------------------------------|
|               stereo tools              |-------------------|--------      
|               - pan                     |  stereo osc LFOs  | pan        
|               - width                   |-------------------| width
| modPan                                  |       ADSRs       |            
| modWidth                                |-------------------|              
|                                         |
|-----------------------------------------|


## Stereo Osc

- properties
  - one wavetable
  - osc tuning offset (coarse and fine together)
  - stereo detune (semitones)
  - phase offset (-1..1)
- methods
  - wavetable types
  - osc tuning offset (semitones)
  - 
  - stereo detuning(semitones)
- internal nodes
  - Gain<1> tuning mod
  - Gain<1> stereo detune mod
  - Gain<1> amplitude mod
- other considerations
  - negative frequency lerp
    - at freq assignment, lerpMethod = (freq < 0) ? lerpNegative : lerpPositive
    - in tick: lerpMethod(a,b)


