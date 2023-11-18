# Thread Synchronization

## approaches:
* counting semaphore
* latch
* barrier

## worker function
Executing a worker function in a thread via lamdba function
|------------------------------------------|
| 100,000 iterations                       |
|------------------------------------------|
| total:      38844055 microseconds        |
| iteration:  38790378 microseconds        |
| iteration:       387 microseconds average|
|------------------------------------------|
| semaphore:       124 microseconds average|
| latch:           120 microseconds average|
| barrier:         137 microseconds average|
|------------------------------------------|
| semaphore:  12425591 microseconds        |
| latch:      12082630 microseconds        |
| barrier:    13769721 microseconds        |
|------------------------------------------|

## worker class method
Executing a worker class method in a thread via lamdba function

|------------------------------------------|
| 1,000,000 iterations                     |
|------------------------------------------|
| total:     395665377 microseconds        |
| iteration: 395125848 microseconds        |
| iteration:       395 microseconds average|
|------------------------------------------|
| semaphore:       125 microseconds average|
| latch:           124 microseconds average|
| barrier:         143 microseconds average|
|------------------------------------------|
| semaphore: 125420776 microseconds        |
| latch:     124811474 microseconds        |
| barrier:   143780787 microseconds        |
|------------------------------------------|


## pure overhead measurement
removed the printf statement from the main loop

|------------------------------------------|
| 10,000 iterations                        |
|                                          |
| workers:               6                 |
| synchronization depth: 3                 |
|------------------------------------------|
| total:     3528996 microseconds          |
| iteration: 3523638 microseconds          |
| iteration:     352 microseconds average  |
|------------------------------------------|
| semaphore:     112 microseconds average  |
| latch:         110 microseconds average  |
| barrier:       128 microseconds average  |
|------------------------------------------|
| semaphore: 1124584 microseconds          |
| latch:     1106021 microseconds          |
| barrier:   1282013 microseconds          |
|---------------------------------------------|
| Cost per worker                             |
| semaphore:       18.67 microseconds average |
| latch:           18.33 microseconds average |
| barrier:         21.33 microseconds average |
|---------------------------------------------|
| Cost per layer of depth                     |
| semaphore:       37.33 microseconds average |
| latch:           36.67 microseconds average |
| barrier:         42.67 microseconds average |
|---------------------------------------------|


|-------------------------------------------|
| 10,000 iterations                         |
|                                           |
| workers:               111                |
| synchronization depth: 3                  |
|-------------------------------------------|
| total:     104214268 microseconds         |
| iteration: 104097597 microseconds         |
| iteration:     10409 microseconds average |
|-------------------------------------------|
| semaphore:     3378 microseconds average  |
| latch:         3394 microseconds average  |
| barrier:       3635 microseconds average  |
|-------------------------------------------|
| semaphore: 33789518 microseconds          |
| latch:     33942720 microseconds          |
| barrier:   36353627 microseconds          |
|---------------------------------------------|
| Cost per worker                             |
| semaphore:   304410.07 microseconds average |
| latch:       305790.27 microseconds average |
| barrier:     327510.15 microseconds average |
|---------------------------------------------|
| Cost per layer of depth                     |
| semaphore: 11263172.67 microseconds average |
| latch:     11314240.00 microseconds average |
| barrier:   12117875.67 microseconds average |
|---------------------------------------------|

There is a huge overhead with creating and joining threads.
Next steps: re-use threads






