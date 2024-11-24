file(GLOB_RECURSE CPP_BOILERPLATE_SOURCE_CPP
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_library(libcpp_boilerplate STATIC ${CPP_BOILERPLATE_SOURCE_CPP})
target_include_directories(libcpp_boilerplate PUBLIC ${CMAKE_CURRENT_LIST_DIR})
