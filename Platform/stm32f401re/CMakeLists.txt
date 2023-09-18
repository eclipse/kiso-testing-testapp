list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" CACHE FORCE)

# Target platform source code
add_subdirectory(src)

# Target platform config
if(APP_OSADAPTER STREQUAL "FreeRtosAdapter")
  #set_property(TARGET stm32cubef4-hal stm32cubef4-usb-device APPEND PROPERTY COMPILE_OPTIONS "-w")
  get_target_property(stm32cubef4-hal-type stm32cubef4-hal TYPE)

  if (NOT stm32cubef4-hal-type STREQUAL "INTERFACE_LIBRARY")
    target_compile_definitions(stm32cubef4-hal PUBLIC register=) # Workaround to disable use of register keyword in public headers as this has been removed from C++17
  endif()
  target_link_libraries(freertos PRIVATE stm32cubef4-cmsis)
  # disable compiler warnings for freertos 
  set_property(TARGET freertos APPEND PROPERTY COMPILE_OPTIONS "-w")
endif()
