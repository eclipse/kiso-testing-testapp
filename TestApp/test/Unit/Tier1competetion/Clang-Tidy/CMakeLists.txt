set(MODULE_NAME "Tier1competetion")

add_library(clang-tidy-${MODULE_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_NAME}ClangTidy.cpp
 )

target_link_libraries(clang-tidy-${MODULE_NAME}
  PRIVATE
    ${MODULE_NAME}
  )

# Checks/Options must be provided as semicolon-separated list inside a string, e.g. "-cppcoreguidelines-non-private-member-variables-in-classes;-misc-non-private-member-variables-in-classes"
set_target_properties(clang-tidy-${MODULE_NAME}
  PROPERTIES
    CLANG_TIDY_CHECKS_C_TARGET ""
    CLANG_TIDY_CHECKS_CXX_TARGET ""
    CLANG_TIDY_CHECK_OPTIONS_C_TARGET ""
    CLANG_TIDY_CHECK_OPTIONS_CXX_TARGET ""
    #enable the option once all warnings are resolved
    #CLANG_TIDY_WARNINGS_AS_ERRORS "*"
  )

# # Adds clang-tidy target
# testapp_setup_clang_tidy_target(
#   MODULE_NAME ${MODULE_NAME}
#   PARENT_TARGET clang-tidy-${MODULE_NAME}
#   INCLUDES
# )