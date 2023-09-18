# get_filename_component(STM32CUBEF4_HAL_BASE "${stm32cubef4_SOURCE_DIR}/Drivers" REALPATH)

# set(STM32CUBEF4_HAL_LOCATION ${STM32CUBEF4_HAL_BASE}/STM32F4xx_HAL_Driver)
# set(STM32CUBEF4_HAL_SOURCES
#   ${STM32CUBEF4_HAL_LOCATION}/Src
#   )

# message(STATUS "stm32cubef4_SOURCE_DIR ${stm32cubef4_SOURCE_DIR}")

if(NOT DEFINED STM32CUBEF4_CONF)
  message(FATAL_ERROR "STM32CUBEF4_CONF not defined.")
endif(NOT DEFINED STM32CUBEF4_CONF)

# string(REPLACE ${stm32cubef4_SOURCE_DIR}  "" STM32CUBEF4_CONF_NO_PROJECT_DIR ${STM32CUBEF4_CONF})
# message("Config directory: ${STM32CUBEF4_CONF_NO_PROJECT_DIR}")

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")

  add_library(stm32cubef4-hal
  ${stm32cubef4_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src
    )

  # the include directories are explicitly marked as non system includes as cmake apparently removes system include paths from the dependency tree
  target_include_directories(stm32cubef4-hal
    PUBLIC
      ${stm32cubef4_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
      ${STM32CUBEF4_CONF}
    )

  target_link_libraries(stm32cubef4-hal
    PUBLIC
      stm32cubef4-cmsis
    )

  target_compile_options(stm32cubef4-hal
    PUBLIC
      #-D${STM32CUBEF4_DEVICE} -DSTM32F401RE 
      -DSTM32F401xE 
      #-D${STM32CUBEF4_CPU_TYPE_U}xx -D${STM32CUBEF4_CPU_FAMILY_U} -D${STM32CUBEF4_CPU_FAMILY_A}
  )
  set_target_properties(stm32cubef4-hal PROPERTIES LINKER_LANGUAGE CXX)

else()

  add_library(stm32cubef4-hal INTERFACE
    )

  # the include directories are explicitly marked as non system includes as cmake apparently removes system include paths from the dependency tree
  target_include_directories(stm32cubef4-hal
    INTERFACE
    ${stm32cubef4_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
      ${STM32CUBEF4_CONF}
    )

  target_link_libraries(stm32cubef4-hal
    INTERFACE
      stm32cubef4-cmsis
    )

  # target_compile_options(stm32cubef4-hal
  #   INTERFACE
  #     -D${STM32CUBEF4_DEVICE} -D${STM32CUBEF4_CPU_TYPE_U}xx -D${STM32CUBEF4_CPU_FAMILY_U} -D${STM32CUBEF4_CPU_FAMILY_A}
  # )

  set_target_properties(stm32cubef4-hal PROPERTIES LINKER_LANGUAGE CXX)

endif()