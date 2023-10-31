# Based on the toolchain for the actual hardware
include(${CMAKE_CURRENT_LIST_DIR}/stm32-arm-gcc.cmake)

# Overwrite Linkerflags: Use generic semihosting instead of the actual MCU
set(CMAKE_EXE_LINKER_FLAGS "--specs=rdimon.specs")

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  # Tell CMake to wrap tests with qemu
  find_program(QEMU_ARM qemu-arm)
  set(CMAKE_CROSSCOMPILING_EMULATOR ${QEMU_ARM})
endif()

# Set cross compiling to ON
set(RUNNING_UNITTESTS_WITH_EMULATOR ON CACHE BOOL "is crosscompiled" FORCE)

# Configure GTest for embedded
add_compile_definitions(arm)
set(gtest_disable_pthreads ON CACHE BOOL "Disable uses of pthreads in gtest." FORCE)

# Include TestApp mock libraries
set(TESTAPP_ENABLE_MOCKS ON)
