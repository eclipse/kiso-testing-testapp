set(MODULE_NAME "TestEnvironment")

add_executable(${MODULE_NAME}-Test
  AssertTest.cpp
  AssertReportTest.cpp
  LoggingTest.cpp
  TestEnvironmentTest.cpp
  )

target_link_libraries(${MODULE_NAME}-Test
  PUBLIC
    MessageHandler
    TestApp-Interface
    TestEnvironment
    etl
    MessageBufferReference-Mock
    gtest
    gmock_main
  )

  # Adds test executable as well target to run the test and generate test results
testapp_add_test(
  MODULE_NAME ${MODULE_NAME}
  )

# Adds clang-tidy target
# testapp_setup_clang_tidy_target(
#  MODULE_NAME ${MODULE_NAME}
#  PARENT_TARGET ${MODULE_NAME}
#  INCLUDES ${PROJECT_SOURCE_DIR}/TestApp/include/${MODULE_NAME}
#  )

# # Adds gcov target
# testapp_add_gcovr(
#   NAME ${MODULE_NAME}
#   SRC ${PROJECT_SOURCE_DIR}/TestApp/src/${MODULE_NAME}
#   INCLUDES ${PROJECT_SOURCE_DIR}/TestApp/include/${MODULE_NAME}
#   DEPENDENCIES Run-${MODULE_NAME}-Test
#   )

# # Adds doxygen target
# testapp_add_doxygen(
#   NAME ${MODULE_NAME}
#   INCLUDES ${PROJECT_SOURCE_DIR}/TestApp/include/${MODULE_NAME}
#   )
