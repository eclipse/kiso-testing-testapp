set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_ASM_FLAGS "")

set(SHARED_LIBS OFF)
set(STATIC_LIBS ON)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT DEFINED ENV{JENKINS_URL})
  # Not running on Jenkins -> enable color output
  set(COMMON_FLAGS "${COMMON_FLAGS} -fdiagnostics-color=always")
endif()

# Compiler Flags
# Note: GNU linker flags to generate mapfiles are set in the testAppAddTest.cmake file and only relevant for test targets
set(COMMON_FLAGS "${COMMON_FLAGS} -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mcpu=cortex-m33 -mthumb -fno-builtin -ffunction-sections -fdata-sections -fomit-frame-pointer -MMD -MP")
set(CMAKE_C_FLAGS "${COMMON_FLAGS} ${DEPFLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} ${DEPFLAGS} -fno-rtti -fno-exceptions")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp")

set(CMAKE_C_COMPILER_TARGET arm-none-eabi)
set(CMAKE_CXX_COMPILER_TARGET arm-none-eabi)

# Target platform device
set(MCU "STM32L562QE")
set(DEVICE_UNDER_TEST_ARCH "ARM")
set(PROJECT_PLATFORM "stm32l562qe") #stm32f401re