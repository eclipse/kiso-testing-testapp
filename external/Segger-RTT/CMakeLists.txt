cmake_minimum_required(VERSION 3.11)

set(RTT_BASE ${CMAKE_CURRENT_LIST_DIR}/RTT)

add_library(rtt
  EXCLUDE_FROM_ALL
  ${RTT_BASE}/SEGGER_RTT.c
  ${RTT_BASE}/SEGGER_RTT_printf.c
  ${RTT_BASE}/SEGGER_RTT_ASM_ARMv7M.S
  )

target_include_directories(rtt SYSTEM
  PUBLIC
    ${RTT_BASE}
    ${RTT_CONF}
  )

target_compile_features(rtt
  PUBLIC
    c_std_99
  )
