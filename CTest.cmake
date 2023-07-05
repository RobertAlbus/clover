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
    test/Algorithm/Envelope/ADSR.cc
    test/Algorithm/Envelope/Basic.cc
    test/Algorithm/Envelope/DC.cc

    test/Algorithm/Delay/Fractional.cc

    test/Graph/Frame_Test.cc
    test/Graph/FrameHistory_Test.cc
    test/Graph/Node_Test.cc

    test/Midi/Notes.cc

    test/NodeSimplex/Basic/Gain_Test.cc
    
    test/NodeSimplex/Envelope/AutomationClip_Test.cc
    
    test/NodeSimplex/Filter/Filter_Test.cc
    
    test/NodeSimplex/StepSequencer/STSQ_Test.cc
    test/NodeSimplex/StepSequencer/STSQ_Pattern_Test.cc
    
    test/NodeSimplex/Stereo/Difference_Test.cc
    test/NodeSimplex/Stereo/Invert_Test.cc
    test/NodeSimplex/Stereo/Pan_Test.cc

    test/NodeSimplex/NodeSmokeTests.cc

    test/Util/Calc_Test.cc
    test/Util/MusicTime.cc
    test/Util/HistoryBuffer_Test.cc

)
target_link_libraries(clover_test
    PRIVATE
        Clover
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(clover_test)