set(MODULE_NAME "MessageHandler")

add_executable(${MODULE_NAME}-Test
  ${MODULE_NAME}Test.cpp
  )

target_include_directories(${MODULE_NAME}-Test
	PUBLIC
		${PROJECT_SOURCE_DIR}/TestApp/include)

target_link_libraries(${MODULE_NAME}-Test
  PUBLIC
    MessageHandler
    gtest
    gmock_main
    etl
  )

  # Adds test executable as well target to run the test and generate test results
testapp_add_test(
  MODULE_NAME ${MODULE_NAME}
  )

# # Adds gcov target
# testapp_add_gcovr(
#   NAME ${MODULE_NAME}
#   SRC ${PROJECT_SOURCE_DIR}/TestApp/src/${MODULE_NAME}
#   INCLUDES ${PROJECT_SOURCE_DIR}/TestApp/include/${MODULE_NAME}
#   DEPENDENCIES Run-${MODULE_NAME}-Test
#   )

# Add clang-tidy scans
add_subdirectory(Clang-Tidy)

# # Adds doxygen target
# testapp_add_doxygen(
#   NAME ${MODULE_NAME}
#   INCLUDES ${PROJECT_SOURCE_DIR}/TestApp/include/${MODULE_NAME}
#   )
