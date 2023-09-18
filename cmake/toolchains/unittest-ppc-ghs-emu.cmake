# Based on the toolchain for the actual hardware
include(${CMAKE_CURRENT_LIST_DIR}/ppc-ghs.cmake)

# Tell CMake to wrap unit tests with ghs-ppc emulator
set(CMAKE_CROSSCOMPILING_EMULATOR simppc -cpu="${PPC_CPU}")
# Set cross compiling to ON
set(RUNNING_UNITTESTS_WITH_EMULATOR ON CACHE BOOL "is crosscompiled" FORCE)

# Include TestApp mock libraries
set(TESTAPP_ENABLE_MOCKS ON)
