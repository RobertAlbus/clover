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
|                                             |
| workers:               6                    |
| synchronization depth: 3                    |
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




