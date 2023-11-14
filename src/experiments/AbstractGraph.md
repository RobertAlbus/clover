# Abstract Graph

## Initial timing of lambda based graph orchestration in a single thread

|1--------------------------------
|     type map | 1021 milliseconds
| instance map |  570 milliseconds
|2--------------------------------
|     type map | 1018 milliseconds
| instance map |  601 milliseconds
|3--------------------------------
|     type map | 1003 milliseconds
| instance map |  583 milliseconds
|4--------------------------------
|     type map | 1073 milliseconds
| instance map |  602 milliseconds
|averages ------------------------
|     type map | 1029 milliseconds 
| instance map |  589 milliseconds <= 42.7% faster
x--------------------------------

## single threaded lambda orchestration with programmatic graph relationships

|------------------------------------------------------|
|where x == (explicit relationships)                   |
|where y == (mapped relationships)                     |
|------------------------------------------------------|
|1 second   |                                          |
|Time taken |     type map [x]:   182 milliseconds     |
|Time taken | instance map [y]:    83 milliseconds     |
|------------------------------------------------------|
|1 second   |                                          |
|Time taken |     type map [y]:   266 milliseconds     |
|Time taken | instance map [y]:    83 milliseconds     |
|------------------------------------------------------|
|1 minute   |                                          |
|Time taken |     type map [y]: 14501 milliseconds     |
|Time taken | instance map [y]:  5433 milliseconds     |
|------------------------------------------------------|


## single threaded but with "distance from edge" sorting
this introduces a little overhead for the map

|----------------------------------------------|
|no map:                                       |
|Time taken |     type map:   2999 milliseconds|
|Time taken | instance map:    193 milliseconds|
|----------------------------------------------|
|with map:                                     |
|Time taken |     type map:   3354 milliseconds|
|Time taken | instance map:    396 milliseconds|
|----------------------------------------------|
|performance cost:                             |
|type     map: 10.58%                          |
|instance map: 51.26%                          |
|----------------------------------------------|


    
Next steps:
- split abstract graph experiment into multiple files
    - `std::map<int, std::vector<AbstractNode*>> groupedNodes;`
- create experiment for pausing one thread to wait for others
- parallelize the processing for all nodes in a given group
    

