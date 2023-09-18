set(MODULE_NAME "OsExtension")

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

target_link_libraries(${MODULE_NAME}-Interface
  INTERFACE
    OsAbstraction-Interface
    Observer-Interface
    etl
  )

add_library(${MODULE_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageBuffer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Semaphore.cpp

  )

target_compile_features(${MODULE_NAME}
  INTERFACE
    cxx_std_14
  )

target_link_libraries(${MODULE_NAME}
  PUBLIC
    ${MODULE_NAME}-Interface
    OsAbstraction
  )

# Checks/Options must be provided as semicolon-separated list inside a string, e.g. "-cppcoreguidelines-non-private-member-variables-in-classes;-misc-non-private-member-variables-in-classes"
set_target_properties(${MODULE_NAME}
  PROPERTIES
    CLANG_TIDY_CHECKS_C_TARGET ""
    CLANG_TIDY_CHECKS_CXX_TARGET ""
    CLANG_TIDY_CHECK_OPTIONS_C_TARGET ""
    CLANG_TIDY_CHECK_OPTIONS_CXX_TARGET ""
  )
