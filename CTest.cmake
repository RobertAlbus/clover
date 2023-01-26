########
# testing
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        3fa7f983c69f780378b4d1ad44d36030ca951ba6 # dec 17 2022 main
)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(clover_test
    test/Midi/Notes.cc

    test/NodeSimplex/Adapter/MapAdapter_Test.cc
    test/NodeSimplex/Adapter/NullAdapter_Test.cc

    test/NodeSimplex/Basic/Gain_Test.cc
    test/NodeSimplex/Basic/Rectifier_Test.cc
    
    test/NodeSimplex/Delay/FractionalDelayLine_Test.cc
    
    test/NodeSimplex/Envelope/ADSR_Test.cc
    test/NodeSimplex/Envelope/BasicEnvelope_Test.cc
    test/NodeSimplex/Envelope/DC_Test.cc

    test/Util/MusicTime.cc

)
target_link_libraries(clover_test
    PRIVATE
        Clover
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(clover_test)