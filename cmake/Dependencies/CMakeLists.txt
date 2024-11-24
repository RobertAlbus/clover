option(CLOVER_FETCH_DEPENDENCIES     "" ON)
option(CLOVER_FETCH_GOOGLE_BENCHMARK "FetchContent for Google Benchmark" ON)
option(CLOVER_FETCH_GOOGLE_TEST      "FetchContent for Google Test"      ON)
option(CLOVER_FETCH_LIBSNDFILE       "FetchContent for libsndfile"       ON)
option(CLOVER_FETCH_PORTAUDIO        "FetchContent for portaudio"        ON)

message("\n\n== Dependencies ==\n")

message("CLOVER_FETCH_DEPENDENCIES:     ${CLOVER_FETCH_DEPENDENCIES}")
message("CLOVER_FETCH_GOOGLE_BENCHMARK: ${CLOVER_FETCH_GOOGLE_BENCHMARK}")
message("CLOVER_FETCH_GOOGLE_TEST:      ${CLOVER_FETCH_GOOGLE_TEST}")
message("CLOVER_FETCH_LIBSNDFILE:       ${CLOVER_FETCH_LIBSNDFILE}")
message("CLOVER_FETCH_PORTAUDIO:        ${CLOVER_FETCH_PORTAUDIO}")

include(GoogleBenchmark.cmake)
include(GoogleTest.cmake)
include(libsndfile.cmake)
include(portaudio.cmake)
message("\n--------\n")

