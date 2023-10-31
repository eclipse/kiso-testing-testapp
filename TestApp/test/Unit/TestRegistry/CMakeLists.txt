set(MODULE_NAME "TestRegistry")

add_executable(${MODULE_NAME}-Test
  ${MODULE_NAME}Test.cpp
  )

target_link_libraries(${MODULE_NAME}-Test
  PUBLIC
    TestApp-Interface
    etl
    gtest
    gmock_main
    TestEnvironment
  )

# Adds test executable as well target to run the test and generate test results
testapp_add_test(
  MODULE_NAME ${MODULE_NAME}
  )


set(MODULE_NAME "TestApp")
add_library(clang-tidy-${MODULE_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/TestRegistryClangTidy.cpp
  )

target_link_libraries(clang-tidy-${MODULE_NAME}
  PUBLIC
    ${MODULE_NAME}-Interface
    MessageHandler
    TestEnvironment
)

#Checks/Options must be provided as semicolon-separated list inside a string, e.g. "-cppcoreguidelines-non-private-member-variables-in-classes;-misc-non-private-member-variables-in-classes"
# set_target_properties(clang-tidy-${MODULE_NAME}
#   PROPERTIES
#     CLANG_TIDY_CHECKS_C_TARGET ""
#     CLANG_TIDY_CHECKS_CXX_TARGET ""
#     CLANG_TIDY_CHECK_OPTIONS_C_TARGET ""
#     CLANG_TIDY_CHECK_OPTIONS_CXX_TARGET ""
#   )

# # Adds clang-tidy target
# testapp_setup_clang_tidy_target(
#   MODULE_NAME ${MODULE_NAME}
#   PARENT_TARGET clang-tidy-${MODULE_NAME}
#   INCLUDES
# )

# # Adds doxygen target
# testapp_add_doxygen(
#   NAME TestRegistry
#   INCLUDES ${PROJECT_SOURCE_DIR}/TestApp/include/TestRegistry ${PROJECT_SOURCE_DIR}/TestApp/include/TestFactory ${PROJECT_SOURCE_DIR}/TestApp/include/TestSuite ${PROJECT_SOURCE_DIR}/TestApp/include/TestCase  
#   )
