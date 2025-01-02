find_program(clover_CTEST_EXECUTABLE ctest)
if(NOT clover_CTEST_EXECUTABLE)
    message(FATAL_ERROR "ctest not found!")
endif()

# set(CMAKE_CTEST_ARGUMENTS "")

file(GLOB_RECURSE CLOVER_TEST_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/**/*.test.cpp
)

add_executable(clover_test ${CLOVER_TEST_SOURCE_CPP})
target_link_libraries(clover_test PRIVATE clover GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(clover_test)
