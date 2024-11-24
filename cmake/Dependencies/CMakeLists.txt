option(CLOVER_FETCH_DEPENDENCIES     "" ON)
option(CLOVER_FETCH_GOOGLE_BENCHMARK "FetchContent for Google Benchmark" ON)
option(CLOVER_FETCH_GOOGLE_TEST      "FetchContent for Google Test"      ON)

message("\n\n== Dependencies ==\n")

message("CLOVER_FETCH_DEPENDENCIES:     ${CLOVER_FETCH_DEPENDENCIES}")
message("CLOVER_FETCH_GOOGLE_BENCHMARK: ${CLOVER_FETCH_GOOGLE_BENCHMARK}")
message("CLOVER_FETCH_GOOGLE_TEST:      ${CLOVER_FETCH_GOOGLE_TEST}")

include(GoogleBenchmark.cmake)
include(GoogleTest.cmake)
message("\n--------\n")

