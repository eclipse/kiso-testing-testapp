include(config/doxygen)

# Doxygen documentation
# \brief Creates doxygen top level and module targets
# \param[in] If not defined top level target Doxygen-testapp is created otherwise the module related target Doxygen-${name}.
function(testapp_add_doxygen)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs INCLUDES)

  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  # Find required packages
  find_package(Doxygen REQUIRED)

  if(DOXYGEN_FOUND)
    # set the path for doxygen output
    set(TESTAPP_DOXYGEN_OUTPUT_DIR "${PROJECT_BINARY_DIR}/integration-testapp/doxygen")

    if(ARG_NAME)
      if(EXISTS "${PROJECT_SOURCE_DIR}/TestApp/src/${ARG_NAME}")
        set(DOXYGEN_SRC_DIR "${DOXYGEN_SRC_DIR}" "${PROJECT_SOURCE_DIR}/TestApp/src/${ARG_NAME}")
      endif()
      if(EXISTS "${PROJECT_SOURCE_DIR}/TestApp/include/${ARG_NAME}")
        set(DOXYGEN_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/TestApp/include/${ARG_NAME}")
      endif()
      set(DOXYGEN_SRC_DIR "${DOXYGEN_SRC_DIR}" ${ARG_INCLUDES})
      set(DOXYGEN_INCLUDE_DIR "${DOXYGEN_INCLUDE_DIR}" ${ARG_INCLUDES})
      set(DOXYGEN_TARGET_NAME "doxygen-${ARG_NAME}")
      set(DOXYGEN_OUTPUT_DIRECTORY "${TESTAPP_DOXYGEN_OUTPUT_DIR}/${ARG_NAME}")
      set(DOXYGEN_WARN_LOGFILE "${TESTAPP_DOXYGEN_OUTPUT_DIR}/${ARG_NAME}/doxygen.log")
      set(DOXYGEN_COMMENT "Generate doxygen documentation for module ${ARG_NAME}")
    else()
    # Set Doxygen project-specific parameters
      set(DOXYGEN_SRC_DIR ${PROJECT_SOURCE_DIR}/TestApp/src ${PROJECT_SOURCE_DIR}/TestApp/include)
      set(DOXYGEN_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/TestApp/include)
      set(DOXYGEN_TARGET_NAME "doxygen-all")
      set(DOXYGEN_OUTPUT_DIRECTORY "${TESTAPP_DOXYGEN_OUTPUT_DIR}/all")
      set(DOXYGEN_WARN_LOGFILE "${TESTAPP_DOXYGEN_OUTPUT_DIR}/all/doxygen.log")
      set(DOXYGEN_COMMENT "Generate doxygen documentation")
      set(DOXYGEN_GENERATE_TAGFILE "${DOXYGEN_OUTPUT_DIRECTORY}/TestApp.tag")
    endif()
    set(DOXYGEN_FILE_PATTERNS ${DOXYGEN_FILE_PATTERNS} "*.md")
    set(DOXYGEN_STRIP_FROM_PATH "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
    set(DOXYGEN_HAVE_DOT "YES")
    set(DOXYGEN_DOTFILE_DIRS "${PROJECT_BINARY_DIR}/integration-testapp/dot/cmake")
    set(DOXYGEN_IMAGE_PATH "${PROJECT_BINARY_DIR}/integration-testapp/dot/cmake")
    # Create doxygen target
    doxygen_add_docs(${DOXYGEN_TARGET_NAME}
      ${DOXYGEN_SRC_DIR}
      ${DOXYGEN_INCLUDE_DIR}
      COMMENT ${DOXYGEN_COMMENT}
    )

    if(TESTAPP_STANDALONE_BUILD)
      add_custom_command(TARGET ${DOXYGEN_TARGET_NAME}
        COMMAND ${CMAKE_COMMAND} -DINPUT_FILE=${DOXYGEN_WARN_LOGFILE} -P ${PROJECT_SOURCE_DIR}/cmake/testAppReadFileAndPrintContent.cmake
      )
    endif()

    if(ARG_NAME)
      add_dependencies(doxygen-testapp ${DOXYGEN_TARGET_NAME})
    endif()
  endif()
endfunction()

