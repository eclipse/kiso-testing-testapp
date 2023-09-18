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

# Target platform device
set(MCU "STM32F401RE")
set(DEVICE_UNDER_TEST_ARCH "ARM")
set(PROJECT_PLATFORM "stm32f401re")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT DEFINED ENV{JENKINS_URL})
  # Not running on Jenkins -> enable color output
  set(COMMON_FLAGS "${COMMON_FLAGS} -fdiagnostics-color=always")
endif()

# Compiler Flags
# Note: GNU linker flags to generate mapfiles are set in the testAppAddTest.cmake file and only relevant for test targets
set(COMMON_FLAGS "${COMMON_FLAGS} -mfloat-abi=softfp -mcpu=cortex-m4 -mfpu=vfpv4 -mthumb -fno-builtin -ffunction-sections -fdata-sections -fomit-frame-pointer -MMD -MP")
#set(COMMON_FLAGS "${COMMON_FLAGS} -mcpu=cortex-m4 -mfloat-abi=softfp -MP -MM -fuse-ld=bfd")
#set(COMMON_FLAGS "${COMMON_FLAGS} -Os -g -Wall  -mcpu=cortex-m4 -mfloat-abi=hard -M") -T ${PROJECT_SOURCE_DIR}/Platform/${PROJECT_PLATFORM}/config/Linker/stm32l562xx.ld
set(CMAKE_C_FLAGS "${COMMON_FLAGS} ${DEPFLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} ${DEPFLAGS} -fno-rtti -fno-exceptions")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp -specs=nosys.specs")

set(CMAKE_C_COMPILER_TARGET arm-none-eabi)
set(CMAKE_CXX_COMPILER_TARGET arm-none-eabi)



# configure_file(${PROJECT_SOURCE_DIR}/Platform/stm32f401re/config/STM32CubeF4/cmake/hal.cmake ${PROJECT_BINARY_DIR}/external/stm32cubeF4-src/cmake/cmsic.hal)
# configure_file(${PROJECT_SOURCE_DIR}/Platform/stm32f401re/config/STM32CubeF4/cmake/cmsic.cmake ${PROJECT_BINARY_DIR}/external/stm32cubeF4-src/cmake/cmsic.cmake)
# configure_file(${PROJECT_SOURCE_DIR}/Platform/stm32f401re/config/STM32CubeF4/cmake/usb-device.cmake ${PROJECT_BINARY_DIR}/external/stm32cubeF4-src/cmake/usb-device.cmake)
# configure_file(${PROJECT_SOURCE_DIR}/Platform/stm32f401re/config/STM32CubeF4/CMakeLists.txt ${PROJECT_BINARY_DIR}/external/stm32cubeF4-src/CMakeLists.txt)
