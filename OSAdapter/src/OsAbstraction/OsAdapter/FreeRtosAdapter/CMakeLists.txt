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
    freertos-Interface
    OsAbstraction-Interface
  )

add_library(${MODULE_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/FreeRtosAdapter.cpp
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

target_link_libraries(${MODULE_NAME}
  PUBLIC
    freertos
  )

# Checks/Options must be provided as semicolon-separated list inside a string, e.g. "-cppcoreguidelines-non-private-member-variables-in-classes;-misc-non-private-member-variables-in-classes"
set_target_properties(${MODULE_NAME}
  PROPERTIES
    CLANG_TIDY_CHECKS_C_TARGET ""
    CLANG_TIDY_CHECKS_CXX_TARGET ""
    CLANG_TIDY_CHECK_OPTIONS_C_TARGET ""
    CLANG_TIDY_CHECK_OPTIONS_CXX_TARGET ""
  )
