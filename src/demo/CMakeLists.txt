file(GLOB_RECURSE CLOVER_DEMO_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(clover_demo ${CLOVER_DEMO_SOURCE_CPP})
target_include_directories(clover_demo PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(clover_demo PRIVATE clover)
