find_package(PkgConfig REQUIRED)
pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)

if(PIPEWIRE_FOUND)
    message(STATUS "PipeWire found: ${PIPEWIRE_VERSION}")
else()
    message(FATAL_ERROR "PipeWire not found - required for audio IO")
endif()
