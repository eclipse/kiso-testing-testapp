set(MODULE_NAME "OsAdapter")

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )

target_compile_features(${MODULE_NAME}-Interface
  INTERFACE
    cxx_std_14
  )

target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include/OsAbstraction
  )

target_link_libraries(${MODULE_NAME}-Interface
  INTERFACE
    OsAbstraction-Interface
  )

target_compile_definitions(${MODULE_NAME}-Interface
  INTERFACE
    DISABLE_FREERTOS
  )

add_library(${MODULE_NAME}
  INTERFACE
  )

target_compile_features(${MODULE_NAME}
  INTERFACE
    cxx_std_14
  )

target_link_libraries(${MODULE_NAME}
  INTERFACE
    OsAbstraction
  )
