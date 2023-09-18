# Helper function for WSL (Windows subsystem for Linux)

# @brief Convert to Windows path if running inside WSL, otherwise returns same path
# @param[in] INPUT_PATH Path to be converted
# @param[in] OUTPUT_PATH_VARIABLE Name of variable to which output path should be written
function(testapp_wsl_windows_path_if_running_wsl)
  set(options "")
  set(oneValueArgs INPUT_PATH OUTPUT_PATH_VARIABLE)
  set(multiValueArgs "")

  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(NOT ARG_INPUT_PATH)
    set(${ARG_OUTPUT_PATH_VARIABLE} "" PARENT_SCOPE)
    return()
  endif()

  if(NOT ARG_OUTPUT_PATH_VARIABLE)
    message(FATAL_ERROR "OUTPUT_PATH_VARIABLE not set.")
  endif()

  if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_HOST_SYSTEM_VERSION MATCHES "Microsoft")
    # Running inside WSL
    execute_process(
      COMMAND wslpath -m ${ARG_INPUT_PATH}
      OUTPUT_VARIABLE OUTPUT
      ERROR_VARIABLE ERROR_OUTPUT
      RESULT_VARIABLE RETURN_VALUE
      )

    if (NOT RETURN_VALUE EQUAL 0)
      message(FATAL_ERROR "Could not convert ${ARG_INPUT_PATH}: ${ERROR_OUTPUT}")
    else()
	  string(STRIP "${OUTPUT}" OUTPUT_STRIPPED)
      set(${ARG_OUTPUT_PATH_VARIABLE} ${OUTPUT_STRIPPED} PARENT_SCOPE)
    endif()

  else()
    set(${ARG_OUTPUT_PATH_VARIABLE} ${ARG_INPUT_PATH} PARENT_SCOPE)
  endif()
endfunction()
