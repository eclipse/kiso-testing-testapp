include(platformLinkerConfiguration)

set(TARGET ${PROJECT_NAME}.elf)
set(MAP_FILE ${PROJECT_BINARY_DIR}/Platform/${PROJECT_PLATFORM}/src/System/${PROJECT_NAME}.map)

add_executable(${TARGET}
  Main.cpp
  Handler.cpp
  syscalls.c
  Isr.cpp
  )

target_compile_features(${TARGET}
  PRIVATE
    cxx_std_14
    c_std_99
  )

target_link_libraries(${TARGET}
  PRIVATE
    stm32cubef4-hal
    freertos
    rtt
    # # Reference test objects to main (this is to avoid Linkage order issue with GNU ld linker) 
    $<TARGET_OBJECTS:IntegrationTests>
    TestApp
    TestAppRtos
  )

# # Set the linker flags 
set(LIBRARY_PATH "${PROJECT_BINARY_DIR}/Platform/${PROJECT_PLATFORM}/config/Linker/")
set(LINKER_SCRIPT ${PROJECT_SOURCE_DIR}/Platform/${PROJECT_PLATFORM}/config/Linker/stm32f4.ld)  

arm_target_set_linkerflags_of_target(
  TARGET ${TARGET}
  LIBRARY_PATH ${LIBRARY_PATH}
  LINKER_SCRIPT ${LINKER_SCRIPT}
  MAP_FILE ${MAP_FILE}
)

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${MAP_FILE})

add_custom_command(
    TARGET ${TARGET}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex ${TARGET} ${PROJECT_NAME}.hex
)
