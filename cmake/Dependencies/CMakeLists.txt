option(CPP_BOILERPLATE_FETCH_DEPENDENCIES     "" ON)
option(CPP_BOILERPLATE_FETCH_GOOGLE_BENCHMARK "FetchContent for Google Benchmark" ON)
option(CPP_BOILERPLATE_FETCH_GOOGLE_TEST      "FetchContent for Google Test"      ON)

message("\n\n== Dependencies ==\n")

message("CPP_BOILERPLATE_FETCH_DEPENDENCIES:     ${CPP_BOILERPLATE_FETCH_DEPENDENCIES}")
message("CPP_BOILERPLATE_FETCH_GOOGLE_BENCHMARK: ${CPP_BOILERPLATE_FETCH_GOOGLE_BENCHMARK}")
message("CPP_BOILERPLATE_FETCH_GOOGLE_TEST:      ${CPP_BOILERPLATE_FETCH_GOOGLE_TEST}")

include(GoogleBenchmark.cmake)
include(GoogleTest.cmake)
message("\n--------\n")

