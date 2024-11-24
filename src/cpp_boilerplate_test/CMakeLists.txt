find_program(CPP_BOILERPLATE_CTEST_EXECUTABLE ctest)
if(NOT CPP_BOILERPLATE_CTEST_EXECUTABLE)
    message(FATAL_ERROR "ctest not found!")
endif()

# set(CMAKE_CTEST_ARGUMENTS "")

file(GLOB_RECURSE CPP_BOILERPLATE_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(cpp_boilerplate_test ${CPP_BOILERPLATE_SOURCE_CPP})
target_link_libraries(cpp_boilerplate_test PRIVATE libcpp_boilerplate GTest::gtest GTest::gtest_main)

add_test(NAME cpp_boilerplate_test COMMAND cpp_boilerplate_test)
