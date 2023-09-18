# Create a target to build-only all unit tests 
add_custom_target(build-unit-tests-all)

# Create a target to run all unit tests
if(RUNNING_UNITTESTS_WITH_EMULATOR)
  # create a target without appending commands since the emulator invokes it's own test suit 
  add_custom_target(run-unit-tests-all)
else()
  add_custom_target(run-unit-tests-all
    COMMAND ${CMAKE_CTEST_COMMAND}
    COMMAND gcc --version
    )
endif()

# \brief Creates a test executable and a target to execute the test and to generate the gtest results as xml-format
# \param[in] MODULE_NAME module name defined in the corresponding CMakeLists.txt
# \param[in] XML_PATH Path to put the XML file, that contains the test results
function(testapp_add_test)
  set(options)
  set(oneValueArgs MODULE_NAME XML_PATH)

  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")
  if (ARG_MODULE_NAME)
    # Generate mapfile for test target when using GNU toolchain (gnu-gcc or arm-none-eabi-gcc)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      target_link_options(${ARG_MODULE_NAME}-Test
        PRIVATE 
          "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${ARG_MODULE_NAME}-Test.map"
      )
    endif()

    add_test(NAME ${ARG_MODULE_NAME}-Test COMMAND ./${ARG_MODULE_NAME}-Test)
    
    add_custom_target(
      Run-${ARG_MODULE_NAME}-Test
      COMMAND ${ARG_MODULE_NAME}-Test --gtest_color=yes
      DEPENDS ${ARG_MODULE_NAME}-Test
      )

    add_dependencies(run-unit-tests-all Run-${ARG_MODULE_NAME}-Test)
    add_dependencies(build-unit-tests-all ${ARG_MODULE_NAME}-Test)
  endif()
endfunction()
