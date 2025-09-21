set(CLOVER_FETCH_LIBSAMPLERATE_VERSION "2ccde9568cca73c7b32c97fefca2e418c16ae5e3")
set(CLOVER_FETCH_LIBSAMPLERATE_VERSION_TAG "${CLOVER_FETCH_LIBSAMPLERATE_VERSION}")

if(CLOVER_FETCH_DEPENDENCIES AND CLOVER_FETCH_LIBSAMPLERATE)
    include(FetchContent)

    message("\n-- LIBSAMPLERATE (${CLOVER_FETCH_LIBSAMPLERATE_VERSION} via FetchContent)")

    option(LIBSAMPLERATE_INSTALL  OFF)
    option(LIBSAMPLERATE_EXAMPLES OFF)
    option(BUILD_TESTING          OFF)

    FetchContent_Declare(
        samplerate
        GIT_REPOSITORY https://github.com/libsndfile/libsamplerate.git
        GIT_TAG ${CLOVER_FETCH_LIBSAMPLERATE_VERSION_TAG}
    )

    set(OLD_CMAKE_MESSAGE_LOG_LEVEL ${CMAKE_MESSAGE_LOG_LEVEL})
    set(CMAKE_MESSAGE_LOG_LEVEL WARNING)

    FetchContent_MakeAvailable(samplerate)

    # Disable clang-tidy for this dependency
    set_target_properties(samplerate PROPERTIES CXX_CLANG_TIDY "")

    set(CMAKE_MESSAGE_LOG_LEVEL ${OLD_CMAKE_MESSAGE_LOG_LEVEL})

endif()
