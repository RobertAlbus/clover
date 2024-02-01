#--------------------------------
# testing

if(CLOVER_TEST_CMAKE_INCLUDE_GUARD)
  return()
endif()
set(CLOVER_TEST_CMAKE_INCLUDE_GUARD TRUE INTERNAL "Inclusion guard for test/CMakeLists.txt")

find_program(CTEST_EXECUTABLE ctest)
if(NOT CTEST_EXECUTABLE)
    message(FATAL_ERROR "ctest not found!")
endif()

enable_testing()
find_package(GTest CONFIG REQUIRED)

file(GLOB_RECURSE SOURCE_LIST_TEST_FUNCTIONAL_CC
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/functional/*.cc
    ${CMAKE_CURRENT_LIST_DIR}/functional/*.h
)

file(GLOB_RECURSE SOURCE_LIST_TEST_PERFORMANCE_CC
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/performance/*.cc
    ${CMAKE_CURRENT_LIST_DIR}/performance/*.h
)

add_executable(clover_test ${SOURCE_LIST_TEST_FUNCTIONAL_CC})
target_include_directories(clover_test PUBLIC ${CLOVER_INCLUDE_DIRECTORIES})
target_link_libraries(clover_test
    PRIVATE
        Clover
        GTest::gtest GTest::gtest_main
)

add_executable(clover_test_performance ${SOURCE_LIST_TEST_PERFORMANCE_CC})
target_include_directories(clover_test_performance PUBLIC ${CLOVER_INCLUDE_DIRECTORIES})
target_link_libraries(clover_test_performance
    PRIVATE
        Clover
        GTest::gtest GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(clover_test
                     PROPERTIES
                     LABELS "unit")

gtest_discover_tests(clover_test_performance
                     PROPERTIES
                     LABELS "performance")