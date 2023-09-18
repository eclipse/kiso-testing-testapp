# Options to be defined by application:
#
# * STM32CUBEF4_DEVICE
#   Allowed values: STM32F462QE
#
# * STM32CUBEF4_CONF
#   Directory where to put configuration headers
#   Example: ${PROJECT_SOURCE_DIR}/config/STM32CubeF4
#
# * STM32CUBEF4_CMSIS_STARTUP_FILE
#   Allowed values: startup_stm32f462xx.s
#
# * STM32CUBEF4_USBD_CLASS
#   Allowed values: AUDIO, CDC, DFU, HID, MSC
#
# * STM32CUBEF4_DOXYGEN_OUTPUT_DIRECTORY
#   Directory where to put doxygen output

cmake_minimum_required(VERSION 3.15)

# if(NOT DEFINED STM32CUBEF4_DEVICE)
#   message(FATAL_ERROR "No processor defined")
# endif(NOT DEFINED STM32CUBEF4_DEVICE)

# string(TOUPPER ${STM32CUBEF4_DEVICE} STM32CUBEF4_DEVICE_U)
# string(TOLOWER ${STM32CUBEF4_DEVICE} STM32CUBEF4_DEVICE_L)

# # Determine STM32CUBEF4_DEVICE family
# string(REGEX MATCH "^(STM32[FL][0-9])" STM32CUBEF4_CPU_FAMILY_U "${STM32CUBEF4_DEVICE_U}")
# string(TOLOWER ${STM32CUBEF4_CPU_FAMILY_U} STM32CUBEF4_CPU_FAMILY_L)

# # Generic families
# string(REGEX MATCH "^(STM32[FL][0-9][0-9][0-9])([A-Z])([A-Z0-9])" STM32CUBEF4_CPU_FAMILY_MATCH "${STM32CUBEF4_DEVICE_U}")
# set(STM32CUBEF4_CPU_FAMILY_A "${CMAKE_MATCH_1}x${CMAKE_MATCH_3}")

# # Determine short device type
# string(REGEX MATCH "^(STM32[FL][0-9][0-9][0-9])" STM32CUBEF4_CPU_TYPE_U "${STM32CUBEF4_DEVICE_U}")
# string(TOLOWER ${STM32CUBEF4_CPU_TYPE_U} STM32CUBEF4_CPU_TYPE_L)

# # Set CPU type for compiler
# if(${STM32CUBEF4_CPU_FAMILY_U} STREQUAL "STM32F4")
#   set(STM32CUBEF4_CPU_TYPE "m4")
# else()
#   message(FATAL_ERROR "Unrecognised device family: ${STM32CUBEF4_CPU_FAMILY_U}")
# endif()

# if(NOT ${STM32CUBEF4_CPU_FAMILY_U} STREQUAL "STM32F4")
#   message(FATAL_ERROR "CPU family ${STM32CUBEF4_CPU_FAMILY_U} not supported.")
# endif()

# # Print configuration
# message("STM32CubeF4 configuration")
# message("          Device: ${STM32CUBEF4_DEVICE}")
# if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
#   message("    Library type: Full (compiling for arm)")
# else()
#   message("    Library type: Interface only (not compiling for arm)")
# endif()

#set(stm32cubeF4_DEVICE ${MCU})
set(STM32CUBEF4_CONF ${PROJECT_SOURCE_DIR}/Platform/stm32f401re/config/stm32cubeF4)

include(${stm32cubef4_SOURCE_DIR}/cmake/cmsis.cmake)
include(${stm32cubef4_SOURCE_DIR}/cmake/hal.cmake)
##here
#include(${CMAKE_CURRENT_LIST_DIR}/cmake/usb-device.cmake)

# Doxygen documentation
# find_package(Doxygen)

# if(DOXYGEN_FOUND)
#   # Create new scope to avoid setting DOXYGEN_* variables in top level scope
#   function(stm32cubef4_add_doxygen)
#     # Include Doxygen default parameters
#     include(${PROJECT_SOURCE_DIR}/external/Toolchain/cmake/config/doxygen.cmake)

#     # Set Doxygen project-specific parameters
#     if(STM32CUBEF4_DOXYGEN_OUTPUT_DIRECTORY)
#       set(DOXYGEN_OUTPUT_DIRECTORY ${STM32CUBEF4_DOXYGEN_OUTPUT_DIRECTORY})
#     else()
#       set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen")
#     endif()
#     set(DOXYGEN_WARN_LOGFILE "${DOXYGEN_OUTPUT_DIRECTORY}/doxygen.log")
#     set(DOXYGEN_STRIP_FROM_PATH "${PROJECT_SOURCE_DIR}")

#     set(DOXYGEN_PROJECT_BRIEF "")
#     set(DOXYGEN_PROJECT_NAME "STM32CubeF4")
#     set(DOXYGEN_PROJECT_NUMBER "")
#     set(DOXYGEN_GENERATE_TAGFILE "${DOXYGEN_OUTPUT_DIRECTORY}/stm32cubef4.tag")

#     # Get files for CMSIS
#     get_target_property(CMSIS_TYPE stm32cubef4-cmsis TYPE)
#     if (CMSIS_TYPE STREQUAL "INTERFACE_LIBRARY")
#       get_target_property(CMSIS_SOURCES stm32cubef4-cmsis INTERFACE_SOURCES)
#       if (CMSIS_SOURCES STREQUAL "CMSIS_SOURCES-NOTFOUND")
#         set(CMSIS_SOURCES "")
#       endif()
#       get_target_property(CMSIS_INCLUDE_DIRECTORIES stm32cubef4-cmsis INTERFACE_INCLUDE_DIRECTORIES)
#     else()
#       get_target_property(CMSIS_SOURCES stm32cubef4-cmsis SOURCES)
#       get_target_property(CMSIS_INCLUDE_DIRECTORIES stm32cubef4-cmsis INCLUDE_DIRECTORIES)
#     endif()

#     # Get files for HAL
#     get_target_property(HAL_TYPE stm32cubef4-hal TYPE)
#     if (HAL_TYPE STREQUAL "INTERFACE_LIBRARY")
#       get_target_property(HAL_SOURCES stm32cubef4-hal INTERFACE_SOURCES)
#       if (HAL_SOURCES STREQUAL "HAL_SOURCES-NOTFOUND")
#         set(HAL_SOURCES "")
#       endif()
#       get_target_property(HAL_INCLUDE_DIRECTORIES stm32cubef4-hal INTERFACE_INCLUDE_DIRECTORIES)
#     else()
#       get_target_property(HAL_SOURCES stm32cubef4-hal SOURCES)
#       get_target_property(HAL_INCLUDE_DIRECTORIES stm32cubef4-hal INCLUDE_DIRECTORIES)
#     endif()

#     # Get files for USB device
#     get_target_property(USB_DEVICE_TYPE stm32cubef4-usb-device TYPE)
#     if (USB_DEVICE_TYPE STREQUAL "INTERFACE_LIBRARY")
#       get_target_property(USB_DEVICE_SOURCES stm32cubef4-usb-device INTERFACE_SOURCES)
#       if (USB_DEVICE_SOURCES STREQUAL "USB_DEVICE_SOURCES-NOTFOUND")
#         set(USB_DEVICE_SOURCES "")
#       endif()
#       get_target_property(USB_DEVICE_INCLUDE_DIRECTORIES stm32cubef4-usb-device INTERFACE_INCLUDE_DIRECTORIES)
#     else()
#       get_target_property(USB_DEVICE_SOURCES stm32cubef4-usb-device SOURCES)
#       get_target_property(USB_DEVICE_INCLUDE_DIRECTORIES stm32cubef4-usb-device INCLUDE_DIRECTORIES)
#     endif()

#     doxygen_add_docs(
#       doxygen-stm32cubef4
#       ${CMSIS_SOURCES} ${CMSIS_INCLUDE_DIRECTORIES} ${HAL_SOURCES} ${HAL_INCLUDE_DIRECTORIES} ${USB_DEVICE_SOURCES} ${USB_DEVICE_INCLUDE_DIRECTORIES}
#       COMMENT "Generate doxygen documentation"
#       )

#     set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${DOXYGEN_OUTPUT_DIRECTORY})
#   endfunction()

#   stm32cubef4_add_doxygen()

#endif()
