set(MODULE_NAME "Observer")

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )

target_compile_features(${MODULE_NAME}-Interface
  INTERFACE
    cxx_std_14
  )

target_include_directories(${MODULE_NAME}-Interface
  INTERFACE
    ${PROJECT_SOURCE_DIR}/OSAdapter/include/${MODULE_NAME}
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
    ${MODULE_NAME}-Interface
  )
