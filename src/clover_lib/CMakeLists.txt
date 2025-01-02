file(GLOB_RECURSE CLOVER_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_library(clover STATIC ${CLOVER_SOURCE_CPP})
target_include_directories(clover PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(clover PRIVATE
    portaudio
    sndfile
    samplerate
)

set_target_properties(clover PROPERTIES
  OUTPUT_NAME "clover"
  DEFINE_SYMBOL clover_EXPORTS
)
