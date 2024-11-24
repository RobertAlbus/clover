option(CPP_BOILERPLATE_ENABLE_CPPCHECK   "Enable static analysis with cppcheck"   ON)
option(CPP_BOILERPLATE_ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" ON)

message("\n\n== Static Analysis ==\n")
message("CPP_BOILERPLATE_ENABLE_CPPCHECK:   ${CPP_BOILERPLATE_ENABLE_CPPCHECK}")
message("CPP_BOILERPLATE_ENABLE_CLANG_TIDY: ${CPP_BOILERPLATE_ENABLE_CLANG_TIDY}")

if(CPP_BOILERPLATE_ENABLE_CPPCHECK)
  find_program(CPP_BOILERPLATE_CPPCHECK_FOUND cppcheck)
  if(CPP_BOILERPLATE_CPPCHECK_FOUND)
    # set(CMAKE_CXX_CPPCHECK "${CPPCHECK};--suppress=missingInclude;--enable=all;--inconclusive;--inline-suppr;--error-exitcode=1;--enable=warning,performance,portability")
    set(CMAKE_CXX_CPPCHECK
      "${CPP_BOILERPLATE_CPPCHECK_FOUND}"
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

if(CPP_BOILERPLATE_ENABLE_CLANG_TIDY)
    find_program(CPP_BOILERPLATE_CLANGTIDY_FOUND clang-tidy)
    if(CPP_BOILERPLATE_CLANGTIDY_FOUND)
        set(CMAKE_CXX_CLANG_TIDY "${CPP_BOILERPLATE_CLANGTIDY_FOUND};--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy;}")
    else()
        message(SEND_ERROR "clang-tidy requested but executable not found")
    endif()
endif()


