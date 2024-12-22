file(GLOB_RECURSE CLOVER_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_library(libclover STATIC ${CLOVER_SOURCE_CPP})
target_include_directories(libclover PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(libclover PRIVATE
    portaudio
    sndfile
    samplerate
)
