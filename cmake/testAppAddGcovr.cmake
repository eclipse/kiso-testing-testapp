if(NOT BASH_PATH)
  message(FATAL_ERROR "testAppAddGcovr.cmake: cannot find bash path")
endif()

find_package(Python3 COMPONENTS Interpreter REQUIRED)

# \brief Creates gcovr top level and module targets
# \param[in] Top-level-Target: none
# \param[in] NAME module name defined in the corresponding CMakeLists.txt
# \param[in] DEPENDENCIS list of targets that must be executed to generate all required analysis and object files
if(NOT TARGET gcovr-all)
  add_custom_target(gcovr-all
    COMMAND ${Python3_EXECUTABLE} -m gcovr --version
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/integration-testapp/gcovr/all
    COMMAND ${BASH_PATH} -c "find ${PROJECT_BINARY_DIR}/CMakeFiles -name '*.gcno' -exec rm {} +"
    COMMAND ${Python3_EXECUTABLE} -m gcovr --branches -r ${PROJECT_SOURCE_DIR} --object-directory ${PROJECT_BINARY_DIR} -f ${PROJECT_SOURCE_DIR}/TestApp/src -f ${PROJECT_SOURCE_DIR}/TestApp/include --html-details -o ${PROJECT_BINARY_DIR}/integration-testapp/gcovr/all/index.html --html-title integration-testapp
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    DEPENDS run-unit-tests-all
  )
endif()


function(testapp_add_gcovr)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs DEPENDENCIES SRC INCLUDES)

  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if (ARG_NAME)
    set(GCOVR_TARGET_NAME gcovr-${ARG_NAME})
    list(TRANSFORM ARG_INCLUDES PREPEND -f\;)
    list(TRANSFORM ARG_SRC PREPEND -f\;)

    add_custom_target(${GCOVR_TARGET_NAME}
      COMMAND ${Python3_EXECUTABLE} -m gcovr --version
      COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/integration-testapp/gcovr/${ARG_NAME}
      COMMAND ${BASH_PATH} -c "find ${PROJECT_BINARY_DIR}/CMakeFiles -name '*.gcno' -exec rm {} +"
      COMMAND ${Python3_EXECUTABLE} -m gcovr --branches -r ${PROJECT_SOURCE_DIR} --object-directory ${PROJECT_BINARY_DIR} ${ARG_SRC} ${ARG_INCLUDES} --html-details -o ${PROJECT_BINARY_DIR}/integration-testapp/gcovr/${ARG_NAME}/index.html --html-title ${ARG_NAME}
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
      DEPENDS ${ARG_DEPENDENCIES}
    )
  endif()
endfunction()