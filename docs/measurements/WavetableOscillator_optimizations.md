WavetableOscillator with fmod()

milliseconds:    353377
milliseconds:    358508
milliseconds:    356090
milliseconds:    352978
milliseconds:    356022
milliseconds:    355480
milliseconds:    355191
milliseconds:    357819
milliseconds:    361883
milliseconds:    367033

average: 357438.1

replace fmod with while{subtract}

milliseconds:    274649
milliseconds:    301754
milliseconds:    285360
milliseconds:    276212
milliseconds:    287448

average: 285084.6

================

WavetableOscillator with remainder()

milliseconds:    272159
milliseconds:    275597
milliseconds:    279764
milliseconds:    274115
milliseconds:    279189

average: 276164.8

replace remainder with while{subtract}

milliseconds:    195047
milliseconds:    198401
milliseconds:    196608
milliseconds:    201659
milliseconds:    199436

average: 198230.2

replace remainder with int-cast subtract

milliseconds:    192338
milliseconds:    195494
milliseconds:    197352
milliseconds:    198319
milliseconds:    197285

average: 196157.6


================

remove .size() call from frame init

milliseconds:    195900
milliseconds:    196893
milliseconds:    198034
milliseconds:    196080
milliseconds:    196018

average: 196585

control:

milliseconds195756
milliseconds:    196868
milliseconds:    197056
milliseconds:    197163
milliseconds:    197238

average: 196816.2

conclusion: results not statistically significant

================