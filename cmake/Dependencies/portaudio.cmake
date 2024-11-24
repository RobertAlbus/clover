set(CLOVER_FETCH_PORTAUDIO_VERSION "cf218ed8e3085ac3731106d3636c3c6396ec2d82")
set(CLOVER_FETCH_PORTAUDIO_VERSION_TAG "${CLOVER_FETCH_PORTAUDIO_VERSION}")

if(CLOVER_FETCH_DEPENDENCIES AND CLOVER_FETCH_PORTAUDIO)
    include(FetchContent)

    message("\n-- PortAudio (${CLOVER_FETCH_PORTAUDIO_VERSION} via FetchContent)")

    option(PA_BUILD_SHARED_LIBS OFF)
    option(PA_BUILD_TESTS       OFF)
    option(PA_BUILD_EXAMPLES    OFF)

    option(PA_USE_ALSA           ON)
    option(PA_USE_JACK           ON)
    option(PA_USE_OSS           OFF)
    option(PA_USE_PULSEAUDIO     ON)
    option(PA_USE_SNDIO         OFF)

    set(old_CMAKE_MESSAGE_LOG_LEVEL ${CMAKE_MESSAGE_LOG_LEVEL})
    set(CMAKE_MESSAGE_LOG_LEVEL ERROR)

    get_property(pre_targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)

    FetchContent_Declare(
        portaudio
        GIT_REPOSITORY git@github.com:PortAudio/portaudio.git
        GIT_TAG ${CLOVER_FETCH_PORTAUDIO_VERSION_TAG}
    )

    FetchContent_MakeAvailable(portaudio)
    
    get_property(post_targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
    foreach(target IN LISTS post_targets)
        if(NOT target IN_LIST pre_targets)
            message(STATUS "New target added: ${target}")
        endif()
    endforeach()


            # Disable clang-tidy for this dependency
            # set_target_properties(portaudio PROPERTIES CXX_CLANG_TIDY "")

    set(CMAKE_MESSAGE_LOG_LEVEL ${old_CMAKE_MESSAGE_LOG_LEVEL})

endif()
