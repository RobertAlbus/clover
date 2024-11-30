option(CLOVER_ENABLE_CPPCHECK     "Enable static analysis with cppcheck"   ON)
option(CLOVER_ENABLE_CLANG_TIDY   "Enable static analysis with clang-tidy" ON)
option(CLOVER_ENABLE_CLANG_FORMAT "Enable linting with clang-format"       ON)

message("\n\n== Static Analysis ==\n")
message("CLOVER_ENABLE_CPPCHECK:     ${CLOVER_ENABLE_CPPCHECK}")
message("CLOVER_ENABLE_CLANG_TIDY:   ${CLOVER_ENABLE_CLANG_TIDY}")
message("CLOVER_ENABLE_CLANG_FORMAT: ${CLOVER_ENABLE_CLANG_FORMAT}")

if(CLOVER_ENABLE_CPPCHECK)
  find_program(CLOVER_CPPCHECK_FOUND cppcheck)
  if(CLOVER_CPPCHECK_FOUND)
    # set(CMAKE_CXX_CPPCHECK "${CPPCHECK};--suppress=missingInclude;--enable=all;--inconclusive;--inline-suppr;--error-exitcode=1;--enable=warning,performance,portability")
    set(CMAKE_CXX_CPPCHECK
      "${CLOVER_CPPCHECK_FOUND}"
      "--enable=warning"
      "--error-exitcode=1"
      "--inconclusive"
      "--inline-suppr"
      "--quiet"
      "--std=c++${CMAKE_CXX_STANDARD}"
      "--template={file}:{line}:({severity}):{id}: {message}"
      "--max-configs=1"
      "--library=googletest"
      # "--cppcheck-build-dir=${CPPCHECK_BUILD_DIR}"
      "--suppressions-list=${CMAKE_SOURCE_DIR}/.cppcheck/suppressions.txt"
    )
  else()
    message(SEND_ERROR "cppcheck requested but executable not found")
  endif()
endif()

if(CLOVER_ENABLE_CLANG_TIDY)
    find_program(CLOVER_CLANG_TIDY_FOUND clang-tidy)
    if(CLOVER_CLANG_TIDY_FOUND)
        set(CMAKE_CXX_CLANG_TIDY "${CLOVER_CLANG_TIDY_FOUND};--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy;}")
    else()
        message(SEND_ERROR "clang-tidy requested but executable not found")
    endif()
endif()

if(CLOVER_ENABLE_CLANG_FORMAT)
    find_program(CLOVER_CLANG_FORMAT_FOUND clang-format)
    if(CLOVER_CLANG_FORMAT_FOUND)
        file(GLOB_RECURSE FMT_SOURCE_LIST
            CONFIGURE_DEPENDS
            ${CMAKE_SOURCE_DIR}/src/*.cpp
            ${CMAKE_SOURCE_DIR}/src/*.hpp
            ${CMAKE_SOURCE_DIR}/src/*.h
        )

        add_custom_target(fmt ALL COMMAND ${CLOVER_CLANG_FORMAT_FOUND} -style=file:${CMAKE_SOURCE_DIR}/.clang-format -i ${FMT_SOURCE_LIST})
    else()
        message(SEND_ERROR "clang-format requested but executable not found")
    endif()
endif()


