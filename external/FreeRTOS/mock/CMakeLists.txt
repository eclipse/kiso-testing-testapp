set(MODULE_NAME "freertos-Mock")

add_library(${MODULE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRtosMock.cpp
  )

target_include_directories(${MODULE_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
  )

target_link_libraries(${MODULE_NAME}
  PUBLIC
    freertos-Interface
    gmock
  )
