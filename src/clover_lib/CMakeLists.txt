# Collect source files (excluding test files)
file(GLOB_RECURSE CLOVER_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)
list(FILTER CLOVER_SOURCE_CPP EXCLUDE REGEX ".*\\.test\\.cpp$")

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

# Test configuration (only if tests are enabled)
if(CLOVER_BUILD_TESTS)
    find_program(clover_CTEST_EXECUTABLE ctest)
    if(NOT clover_CTEST_EXECUTABLE)
        message(FATAL_ERROR "ctest not found!")
    endif()

    # Collect test files
    file(GLOB_RECURSE CLOVER_TEST_SOURCE_CPP
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/*.test.cpp
    )

    add_executable(clover_test ${CLOVER_TEST_SOURCE_CPP})
    target_link_libraries(clover_test PRIVATE clover GTest::gtest GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(clover_test)
endif()
