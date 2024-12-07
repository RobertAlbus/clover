set(CLOVER_FETCH_LIBSNDFILE_VERSION "0d3f80b7394368623df558d8ba3fee6348584d4d")
set(CLOVER_FETCH_LIBSNDFILE_VERSION_TAG "${CLOVER_FETCH_LIBSNDFILE_VERSION}")

if(CLOVER_FETCH_DEPENDENCIES AND CLOVER_FETCH_LIBSNDFILE)
    include(FetchContent)

    message("\n-- libsndfile (${CLOVER_FETCH_LIBSNDFILE_VERSION} via FetchContent)")

    option(BUILD_SHARED_LIBS OFF)
    option(BUILD_PROGRAMS    OFF)
    option(BUILD_EXAMPLES    OFF)
    option(BUILD_TESTING     OFF)

    FetchContent_Declare(
        sndfile
        GIT_REPOSITORY https://github.com/libsndfile/libsndfile.git
        GIT_TAG ${CLOVER_FETCH_LIBSNDFILE_VERSION_TAG}
    )

    set(old_CMAKE_MESSAGE_LOG_LEVEL ${CMAKE_MESSAGE_LOG_LEVEL})
    set(CMAKE_MESSAGE_LOG_LEVEL WARNING)

    FetchContent_MakeAvailable(sndfile)

    # Disable clang-tidy for this dependency
    set_target_properties(sndfile PROPERTIES CXX_CLANG_TIDY "")

    set(CMAKE_MESSAGE_LOG_LEVEL ${old_CMAKE_MESSAGE_LOG_LEVEL})

endif()
