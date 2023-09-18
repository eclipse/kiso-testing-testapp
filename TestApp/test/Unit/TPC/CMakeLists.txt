set(MODULE_NAME "TPC")

add_executable(${MODULE_NAME}-Test
  ${MODULE_NAME}Test.cpp
  )

target_link_libraries(${MODULE_NAME}-Test
  PUBLIC
  MessageHandler
  TestApp-Interface
  TPC
  CommunicationChannel-Mock
  MessageBufferReference-Mock
  OsExtension-Mock
  # Third Party Dependencies for TestApp
  etl
  gtest
  gmock_main
  )

  # Adds test executable as well target to run the test and generate test results
testapp_add_test(
  MODULE_NAME ${MODULE_NAME}
  )

  # testapp_add_gcovr(
  # NAME ${MODULE_NAME}
  # SRC ${PROJECT_SOURCE_DIR}/TestApp/src/${MODULE_NAME}
  # INCLUDES ${PROJECT_SOURCE_DIR}/TestApp/include/${MODULE_NAME}
  # DEPENDENCIES Run-${MODULE_NAME}-Test
  # )

# Add pseudo cpp to enable clang-tidy scans
add_subdirectory(Clang-Tidy)

# # Adds doxygen target
# testapp_add_doxygen(
#   NAME ${MODULE_NAME}
#   INCLUDES ${PROJECT_SOURCE_DIR}/TestApp/include/${MODULE_NAME}
#   )
