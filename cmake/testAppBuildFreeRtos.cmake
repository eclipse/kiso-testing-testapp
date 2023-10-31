cmake_minimum_required(VERSION 3.11)

# Optional FreeRTOS-Plus modules
set(FREERTOS_PLUS_CLI           OFF CACHE BOOL "Support for FreeRTOS-PLUS-CLI")

if(${ARCH} STREQL "GCC")
    set(FREERTOS_BASE ${CMAKE_CURRENT_BINARY_DIR}/FreeRTOS/Source)
set(FREERTOS_PLUS_BASE ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Plus/Source)

set(FREERTOS_SOURCES
  ${FREERTOS_BASE}/croutine.c
  ${FREERTOS_BASE}/event_groups.c
  ${FREERTOS_BASE}/list.c
  ${FREERTOS_BASE}/queue.c
  ${FREERTOS_BASE}/stream_buffer.c
  ${FREERTOS_BASE}/tasks.c
  ${FREERTOS_BASE}/timers.c
  )

# Declare empty lists to make sure they can be appended by optional FreeRTOS-Plus modules
set(FREERTOS_PLUS_SOURCES "")
set(FREERTOS_PLUS_INCLUDES "")

if(FREERTOS_ENABLE_MPU)
  list(APPEND FREERTOS_SOURCES ${FREERTOS_BASE}/portable/Common/mpu_wrappers.c)
endif()

if(NOT DEFINED FREERTOS_PORT)
  message(FATAL_ERROR "No FreeRTOS port defined")
endif(NOT DEFINED FREERTOS_PORT)

if(NOT DEFINED FREERTOS_PORT_BASE)
  set(FREERTOS_PORT_BASE ${FREERTOS_BASE}/portable)
endif(NOT DEFINED FREERTOS_PORT_BASE)

if (NOT EXISTS "${FREERTOS_PORT_BASE}")
  message(FATAL_ERROR "FreeRTOS port directory ${FREERTOS_PORT_BASE} not found")
endif()

# Append base dir to source files
foreach(f ${FREERTOS_PORT_SOURCES})
  list(APPEND FREERTOS_PORT_SOURCES_WITH_PORT_BASE  "${FREERTOS_PORT_BASE}/${FREERTOS_PORT}/${f}")
endforeach(f)

if (FREERTOS_PLUS_CLI)
  list(APPEND FREERTOS_PLUS_SOURCES ${FREERTOS_PLUS_BASE}/FreeRTOS-Plus-CLI/FreeRTOS_CLI.c)
  list(APPEND FREERTOS_PLUS_INCLUDES ${FREERTOS_PLUS_BASE}/FreeRTOS-Plus-CLI/)
endif()

# Create hal library
add_library(freertos
  ${FREERTOS_SOURCES}
  ${FREERTOS_PLUS_SOURCES}
  ${FREERTOS_PORT_SOURCES_WITH_PORT_BASE}
  )

target_include_directories(freertos SYSTEM
  PUBLIC
    ${FREERTOS_BASE}/include
    ${FREERTOS_PORT_BASE}/${FREERTOS_PORT}
    ${FREERTOS_CONF}
    ${FREERTOS_PLUS_INCLUDES}
  )

target_link_libraries(freertos
  PRIVATE
    ${FREERTOS_LINK_LIBRARIES}
  )

add_library(freertos-Interface INTERFACE)

target_include_directories(freertos-Interface
  INTERFACE
    ${FREERTOS_BASE}/include
    ${FREERTOS_PORT_BASE}/${FREERTOS_PORT}
    ${FREERTOS_CONF}
    ${FREERTOS_PLUS_INCLUDES}
  )

add_subdirectory(mock EXCLUDE_FROM_ALL)

unset(FREERTOS_SOURCES)
