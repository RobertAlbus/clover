* [x] add google test and create POC
* [ ] create a harness for exposing Node::tick method
  * templated class, inherits from <T>
  * exposes tick(int) via test_tick(int)
* [ ] create test fixture for a buffer
  * pull samples from the graph (arbitrary quantity)
  * StatelessProcessor<N> type of Node
  * allows a testable Node to be connected to it
  * has a public buffer to aggregate the input values
  * perform assertions against the buffer
* [ ] ??? create a StatelessPRocessor<N> that returns Frame<N>{0, .., N}
  * useful for testing adapters I guess



