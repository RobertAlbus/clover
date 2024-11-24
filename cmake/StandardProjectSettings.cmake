# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(
    STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE
      RelWithDebInfo
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui, ccmake
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()

find_program(clover_CCACHE ccache)
find_program(clover_SCCACHE sccache)
if(clover_CCACHE)
    message("using ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${clover_CCACHE})
else(clover_SCCACHE)
    message("using sccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${clover_SCCACHE})
endif()

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Force generation of compile_commands.json" FORCE)


