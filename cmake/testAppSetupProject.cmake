# Project options.
#        Value = OFF: Disable feature
#        Value = ON: Enable feature
#        Value unset: Enable feature

set(TESTAPP_DOWNLOAD_ONLY OFF CACHE STRING "Return cmake generation after download")

# (TODO)
option(TESTAPP_ENABLE_COMMIT_CHECKS "Enable git pre-push hook to pre-check commit messages" OFF)

# Enable doxygen-all target.
# Requires
#   - set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} <...>/Toolchain-src/cmake)
option(TESTAPP_ENABLE_DOXYGEN "Enable to generate integration-testapp doxygen report" ON)

# Enable gcovr-all target
# Requires following flags: -fprofile-arcs -ftest-coverage
option(TESTAPP_ENABLE_GCOVR "Enable to generate integration-testapp coverage report" ON)

# (TODO) Enable ninja targets to be executed from subfolders. Ninja only
option(TESTAPP_ENABLE_NINJA_ECLIPSE "Enable eclipse target execution from any subfolder" OFF)

# Enable unit-test-all and clang-tidy-all targets and all module targets for clang-tidy, doxygen, gcovr and unit testing.
# Requires:
#   - set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} <...>/Toolchain-src/cmake)
#   - gcovr only: -fprofile-arcs -ftest-coverage
option(TESTAPP_ENABLE_TESTS "Enable building of tests" ON)
if(TESTAPP_ENABLE_TESTS)
  enable_testing()
endif()

# Include SystemCommon mocks
option(TESTAPP_ENABLE_MOCKS "Includes mock libraries" OFF)

if(NOT (CMAKE_CXX_STANDARD AND CMAKE_C_STANDARD))
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_C_STANDARD 99)
endif()

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# include all cmake files to the path
list(APPEND CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/cmake"
  "${PROJECT_SOURCE_DIR}/external/Toolchain/cmake"
)

# integration-testapp Output path
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/integration-testapp)
