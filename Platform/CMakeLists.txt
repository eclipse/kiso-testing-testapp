add_subdirectory(RequiredInterface)

if (NOT(PROJECT_PLATFORM STREQUAL ""))
  add_subdirectory(${PROJECT_PLATFORM})
endif()
