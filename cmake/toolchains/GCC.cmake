# \brief Common modules toolchain file. Uses the gcc compiler for building the unit tests
# \param[in] DCMAKE_MAKE_PROGRAM Path to
# \param[in] DCMAKE_PROGRAM_PATH Program search path

# Set default build type to "Debug", if not otherwise configured via cli
if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Supported types: Debug, Release")
endif()

if(CMAKE_HOST_WIN32)
  set(CMAKE_SYSTEM_NAME windows)
  set(CMAKE_SYSTEM_PROCESSOR x86_64)
endif()

set(CMAKE_C_COMPILER clang CACHE FILEPATH "C Compiler")
set(CMAKE_CXX_COMPILER clang++ CACHE FILEPATH "CXX Compiler")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  set(CMAKE_C_COMPILER_TARGET x86_64-pc-windows-msvc) # x86_64-pc-windows-gnu)
  set(CMAKE_CXX_COMPILER_TARGET x86_64-pc-windows-msvc) # x86_64-pc-windows-gnu)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  set(CMAKE_C_COMPILER_TARGET x86_64-pc-linux-gnu)
  set(CMAKE_CXX_COMPILER_TARGET x86_64-pc-linux-gnu)
endif()

# TODO Remove or at least reduce -03  flag
# The flag let us get around the error "string table overflow at offset 10000001. Cannot close *.cpp.obj: File too big"
# Note: GNU linker flags to generate mapfiles are set in the testAppAddTest.cmake file and only relevant for test targets
set(COMMON_FLAGS "-Wall -Wpedantic -Wextra -Winvalid-pch -O3 -fprofile-arcs -ftest-coverage -fno-exceptions -fno-inline -fdiagnostics-color=always -w")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS}")
set(CMAKE_C_FLAGS "${COMMON_FLAGS}")

set(COMMON_DEBUG_FLAGS "-g -gdwarf-2")
set(CMAKE_C_FLAGS_DEBUG "${COMMON_DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_DEBUG_FLAGS}")

# Include TestApp mock libraries
set(TESTAPP_ENABLE_MOCKS ON)
