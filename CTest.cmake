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
    test/Algorithm/Delay/Fractional.cc

    test/Algorithm/DynamicRange/AsymClip.cc
    test/Algorithm/DynamicRange/UpwardDCCompression.cc

    test/Algorithm/Envelope/ADSR.cc
    test/Algorithm/Envelope/Basic.cc
    test/Algorithm/Envelope/DC.cc

    test/Algorithm/Stereo/Difference.cc
    test/Algorithm/Stereo/MidSideBalance.cc
    test/Algorithm/Stereo/Pan.cc
    test/Algorithm/Stereo/Sum.cc

    test/Algorithm/Wavetable/Generators/Lerp.cc
    test/Algorithm/Wavetable/Generators/NoiseWhite.cc
    test/Algorithm/Wavetable/Generators/Pulse.cc
    test/Algorithm/Wavetable/Generators/Saw.cc
    test/Algorithm/Wavetable/Generators/Sine.cc
    test/Algorithm/Wavetable/Generators/Square.cc
    test/Algorithm/Wavetable/Generators/Tri.cc

    test/Algorithm/Wavetable/OscillatorIndexCalculator.cc
    test/Algorithm/Wavetable/WavetableOscillatorMono.cc

    test/Graph/Frame_Test.cc
    test/Graph/FrameHistory_Test.cc
    test/Graph/Node_Test.cc

    test/Midi/Notes.cc

    test/Nodes/Basic/Gain_Test.cc
    
    test/Nodes/Envelope/AutomationClip_Test.cc
    
    test/Nodes/Filter/Filter_Test.cc
    
    test/Nodes/StepSequencer/STSQ_Test.cc
    test/Nodes/StepSequencer/STSQ_Pattern_Test.cc
    
    test/Nodes/NodeSmokeTests.cc

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