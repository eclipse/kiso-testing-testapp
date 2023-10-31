set(MODULE_NAME "OsAdapter")

add_library(${MODULE_NAME}-Interface
  INTERFACE
  )

target_compile_features(${MODULE_NAME}-Interface
  INTERFACE
    cxx_std_14
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
  ${CMAKE_CURRENT_SOURCE_DIR}/NullOsAdapter.cpp
  )

target_compile_features(${MODULE_NAME}
  INTERFACE
    cxx_std_14
  )

target_link_libraries(${MODULE_NAME}
  PUBLIC
    ${MODULE_NAME}-Interface
  PRIVATE
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
