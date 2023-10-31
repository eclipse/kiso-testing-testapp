# Check if clang-tidy is available
find_program(CLANG_TIDY_PATH clang-tidy)

if(CLANG_TIDY_PATH)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CLANG_TIDY_EXTRA_ARGS "-extra-arg=--stdlib=libstdc++" "-extra-arg=--target=${CMAKE_CXX_COMPILER_TARGET}")
  endif()
  # Include BES3 clang-tidy default options
  include(config/clang-tidy)
else()
  message(FATAL_ERROR "Cannot find clang-tidy path")
endif()

if(NOT TARGET clang-tidy-all)
  add_custom_target(clang-tidy-all COMMAND clang-tidy --version)
endif()

# clang-based tools ignore response files like @CMakeFiles/include.rsp (https://reviews.llvm.org/D34440)
# Deactivate response files for msys so clang-tidy works correctly.
# If you encounter include problems when using clang-tidy and use another generator,
# add it to the condition below.
# Be aware: For long commands (more than 8191 characters), the command line limit can be a problem!
if(CMAKE_GENERATOR MATCHES ".*Makefile.*")
  set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)
  set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES OFF)
  #set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS OFF)
  #set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_LIBRARIES OFF)
endif()

# \brief Create
# \param[in] TARGET name of variable to list of targets
# \param[in] MODULE_NAME name of the module that should be scanned
# \param[in] PARENT_TARGET name of the parent target from which the properties are derived
# \param[in] INCLUDES path of all includes needed by Clang-Tidy target
function(testapp_setup_clang_tidy_target)
  set(options "")
  set(oneValueArgs MODULE_NAME PARENT_TARGET INCLUDES)
  set(multiValueArgs "")

  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  set(CLANG_TIDY_TARGET_NAME clang-tidy-${ARG_MODULE_NAME})
  if(NOT TARGET ${CLANG_TIDY_TARGET_NAME})
    get_target_property(TARGET_SOURCES ${ARG_PARENT_TARGET} SOURCES)
    if(TARGET_SOURCES)
      add_library(${CLANG_TIDY_TARGET_NAME}
        ${TARGET_SOURCES}
        )
    endif()

    get_target_property(MODULE_COMPILE_FLAGS ${ARG_PARENT_TARGET} COMPILE_FLAGS)
    if(MODULE_COMPILE_FLAGS)
      set_target_properties(${CLANG_TIDY_TARGET_NAME}
        PROPERTIES
          COMPILE_FLAGS ${MODULE_COMPILE_FLAGS}
        )
    endif()

    get_target_property(TARGET_LIBRARIES ${ARG_PARENT_TARGET} LINK_LIBRARIES)
    if(TARGET_LIBRARIES)
      target_link_libraries(${CLANG_TIDY_TARGET_NAME}
        PRIVATE
          ${TARGET_LIBRARIES}
        )
    endif()

    get_target_property(TARGET_INCLUDES ${ARG_PARENT_TARGET} INCLUDE_DIRECTORIES)
    if(TARGET_INCLUDES)
      target_include_directories(${CLANG_TIDY_TARGET_NAME}
        PUBLIC
          ${TARGET_INCLUDES} ${ARG_INCLUDES}
        )
    endif()

    get_target_property(TARGET_COMPILE_FEATURES ${ARG_PARENT_TARGET} COMPILE_FEATURES)
    if(TARGET_COMPILE_FEATURES)
      target_compile_features(${CLANG_TIDY_TARGET_NAME}
        PRIVATE
          ${TARGET_COMPILE_FEATURES}
         )
    endif()

    get_target_property(TARGET_COMPILE_DEFINITIONS ${ARG_PARENT_TARGET} COMPILE_DEFINITIONS)
    if(TARGET_COMPILE_DEFINITIONS)
      add_compile_definitions(${TARGET_COMPILE_DEFINITIONS})
    endif()
  endif()
  if(CLANG_TIDY_PATH)
    # Get additional checks specific for target
    get_property(CLANG_TIDY_CHECKS_C_TARGET TARGET ${ARG_PARENT_TARGET} PROPERTY CLANG_TIDY_CHECKS_C_TARGET)
    get_property(CLANG_TIDY_CHECKS_CXX_TARGET TARGET ${ARG_PARENT_TARGET} PROPERTY CLANG_TIDY_CHECKS_CXX_TARGET)
    get_property(CLANG_TIDY_CHECK_OPTIONS_C_TARGET TARGET ${ARG_PARENT_TARGET} PROPERTY CLANG_TIDY_CHECK_OPTIONS_C_TARGET)
    get_property(CLANG_TIDY_CHECK_OPTIONS_CXX_TARGET TARGET ${ARG_PARENT_TARGET} PROPERTY CLANG_TIDY_CHECK_OPTIONS_CXX_TARGET)

    set(CLANG_TIDY_WARNINGS_AS_ERRORS "-warnings-as-errors=")
    get_property(CLANG_TIDY_WARNINGS_AS_ERRORS_TARGET TARGET ${ARG_PARENT_TARGET} PROPERTY CLANG_TIDY_WARNINGS_AS_ERRORS)
    if(CLANG_TIDY_WARNINGS_AS_ERRORS_TARGET)
      set(CLANG_TIDY_WARNINGS_AS_ERRORS "-warnings-as-errors=${CLANG_TIDY_WARNINGS_AS_ERRORS_TARGET}")
    endif()

    # Combine global and target settings
    set(CLANG_TIDY_CHECKS_C_COMBINED ${CLANG_TIDY_CHECKS_C} ${CLANG_TIDY_CHECKS_C_TARGET})
    set(CLANG_TIDY_CHECKS_CXX_COMBINED ${CLANG_TIDY_CHECKS_CXX} ${CLANG_TIDY_CHECKS_CXX_TARGET})
    set(CLANG_TIDY_CHECK_OPTIONS_C_COMBINED ${CLANG_TIDY_CHECK_OPTIONS_C} ${CLANG_TIDY_CHECK_OPTIONS_C_TARGET})
    set(CLANG_TIDY_CHECK_OPTIONS_CXX_COMBINED ${CLANG_TIDY_CHECK_OPTIONS_CXX} ${CLANG_TIDY_CHECK_OPTIONS_CXX_TARGET})

    # Join settings
    list(JOIN CLANG_TIDY_CHECKS_C_COMBINED "," CLANG_TIDY_CHECKS_C_COMBINED_JOINED)
    list(JOIN CLANG_TIDY_CHECKS_CXX_COMBINED "," CLANG_TIDY_CHECKS_CXX_COMBINED_JOINED)
    list(JOIN CLANG_TIDY_CHECK_OPTIONS_C_COMBINED "," CLANG_TIDY_CHECK_OPTIONS_C_COMBINED_JOINED)
    list(JOIN CLANG_TIDY_CHECK_OPTIONS_CXX_COMBINED "," CLANG_TIDY_CHECK_OPTIONS_CXX_COMBINED_JOINED)

    set(CLANG_TIDY_HEADER_FILTER "(${PROJECT_SOURCE_DIR}\/TestApp\/include.*)|(${PROJECT_SOURCE_DIR}\/TestApp\/src.*)")

    set(CLANG_TIDY_CMD_C "${CLANG_TIDY_PATH}" "-header-filter=${CLANG_TIDY_HEADER_FILTER}" "${CLANG_TIDY_WARNINGS_AS_ERRORS}" "-checks=${CLANG_TIDY_CHECKS_C_COMBINED_JOINED}" "-config={CheckOptions: [ ${CLANG_TIDY_CHECK_OPTIONS_C_COMBINED_JOINED} ]}" ${CLANG_TIDY_EXTRA_ARGS})
    set(CLANG_TIDY_CMD_CXX "${CLANG_TIDY_PATH}" "-header-filter=${CLANG_TIDY_HEADER_FILTER}" "${CLANG_TIDY_WARNINGS_AS_ERRORS}" "-checks=${CLANG_TIDY_CHECKS_CXX_COMBINED_JOINED}" "-config={CheckOptions: [ ${CLANG_TIDY_CHECK_OPTIONS_CXX_COMBINED_JOINED} ]}" ${CLANG_TIDY_EXTRA_ARGS})

    set_target_properties(
      ${CLANG_TIDY_TARGET_NAME} PROPERTIES
      CXX_CLANG_TIDY "${CLANG_TIDY_CMD_CXX}"
      C_CLANG_TIDY "${CLANG_TIDY_CMD_C}"
      EXCLUDE_FROM_ALL TRUE
      )

    add_custom_command(
      TARGET ${CLANG_TIDY_TARGET_NAME}
      PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/integration-testapp/clang-tidy/${ARG_PARENT_TARGET}
      )
   add_dependencies(clang-tidy-all ${CLANG_TIDY_TARGET_NAME})
  endif()
endfunction()