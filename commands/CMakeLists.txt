if(CLOVER_COMMANDS_CMAKE_INCLUDE_GUARD)
  return()
endif()
set(CLOVER_COMMANDS_CMAKE_INCLUDE_GUARD TRUE INTERNAL "Inclusion guard for commands/CMakeLists.txt")

include(commands/CMakeLists.command.build.txt)
include(commands/CMakeLists.command.cloc.txt)
include(commands/CMakeLists.command.fmt.txt)
include(commands/CMakeLists.command.profile_call_graph.txt)
include(commands/CMakeLists.command.run.txt)
include(commands/CMakeLists.command.test.txt)
include(commands/CMakeLists.command.tidy.txt)
