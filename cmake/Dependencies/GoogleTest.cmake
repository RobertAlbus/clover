set(CPP_BOILERPLATE_FETCH_GOOGLE_TEST_VERSION "1.15.2")
set(CPP_BOILERPLATE_FETCH_GOOGLE_TEST_VERSION_TAG "v${CPP_BOILERPLATE_FETCH_GOOGLE_TEST_VERSION}")

if(CPP_BOILERPLATE_FETCH_DEPENDENCIES AND CPP_BOILERPLATE_FETCH_GOOGLE_TEST)
    include(FetchContent)

    message("\n-- Google Test (${CPP_BOILERPLATE_FETCH_GOOGLE_TEST_VERSION} via FetchContent)")

    set(BUILD_GMOCK   OFF)
    set(INSTALL_GTEST OFF)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG ${CPP_BOILERPLATE_FETCH_GOOGLE_TEST_VERSION_TAG}
    )

    set(old_CMAKE_MESSAGE_LOG_LEVEL ${CMAKE_MESSAGE_LOG_LEVEL})
    set(CMAKE_MESSAGE_LOG_LEVEL WARNING)

    FetchContent_MakeAvailable(googletest)

    # Disable clang-tidy for this dependency
    set_target_properties(gtest         PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(gtest_main    PROPERTIES CXX_CLANG_TIDY "")

    set(CMAKE_MESSAGE_LOG_LEVEL ${old_CMAKE_MESSAGE_LOG_LEVEL})

else()
    message("\n-- Google Test (${CPP_BOILERPLATE_FETCH_GOOGLE_TEST_VERSION} via find_package)")

    set(old_CMAKE_MESSAGE_LOG_LEVEL ${CMAKE_MESSAGE_LOG_LEVEL})
    set(CMAKE_MESSAGE_LOG_LEVEL WARNING)

    find_package(GTest "${CPP_BOILERPLATE_FETCH_GOOGLE_TEST_VERSION}" REQUIRED CONFIG)

    set(CMAKE_MESSAGE_LOG_LEVEL ${old_CMAKE_MESSAGE_LOG_LEVEL})
endif()
