# \brief Set linker flags for the passed target
#
# \param TARGET target the flags shall be linked to
# \param LIBRARY_PATH path containing the linker script
# \param LINKER_SCRIPT specifies the linker script name (not containing the path) which should be used
# \param MAP_FILE specifies the name of the generated map file
function (arm_target_set_linkerflags_of_target)
  set(options "")
  set(oneValueArgs TARGET LIBRARY_PATH LINKER_SCRIPT MAP_FILE)
  set(multiValueArgs "")
  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  # set custom linker flags
  set_target_properties(${TARGET} PROPERTIES LINK_OPTIONS "SHELL:-specs=nano.specs LINKER:-L${ARG_LIBRARY_PATH},-T${ARG_LINKER_SCRIPT},-Map=${ARG_MAP_FILE},--gc-sections,--cref")

endfunction()
