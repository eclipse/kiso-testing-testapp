# Parses gtest xml output into a JUnit readable format
# \param MODULE_NAME Name of the module that should be displayed
# \param XML_PATH Path to the XML file
# \param PARENT_TARGET Target that executes the custom command
function(testapp_gtest_xml_output_parser)
  set(options)
  set(oneValueArgs MODULE_NAME XML_PATH PARENT_TARGET)

  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  find_package(Python3 COMPONENTS Interpreter REQUIRED)

  # GTEST_XML_PARSER_PATH is set within testAppPullDependencies.cmake after download
  add_custom_command(
    TARGET ${ARG_PARENT_TARGET}
    POST_BUILD
    COMMAND ${Python3_EXECUTABLE} ${GTEST_XML_PARSER_PATH} -x ${ARG_XML_PATH} -m ${ARG_MODULE_NAME}
    )

endfunction()
