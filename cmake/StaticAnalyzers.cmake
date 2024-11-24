option(CLOVER_ENABLE_CPPCHECK   "Enable static analysis with cppcheck"   ON)
option(CLOVER_ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" ON)

message("\n\n== Static Analysis ==\n")
message("CLOVER_ENABLE_CPPCHECK:   ${CLOVER_ENABLE_CPPCHECK}")
message("CLOVER_ENABLE_CLANG_TIDY: ${CLOVER_ENABLE_CLANG_TIDY}")

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


