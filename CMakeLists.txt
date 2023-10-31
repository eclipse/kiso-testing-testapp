cmake_minimum_required(VERSION 3.15)

project (TestApp C CXX)

set(PROJECT_DESCRIPTION "TestApp Software")

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(TESTAPP_STANDALONE_BUILD TRUE)
else()
  set(TESTAPP_STANDALONE_BUILD "")
endif()

set(RUNNING_UNITTESTS_WITH_EMULATOR OFF CACHE BOOL "is crosscompiled")

if("${CMAKE_PROJECT_NAME}" STREQUAL "TestApp")
  if("${DEVICE_UNDER_TEST_ARCH}" STREQUAL "")
    set(PROJECT_PLATFORM "")
    set(DEVICE_UNDER_TEST_ARCH "")
  endif()
endif()

# Setup Project configuration and pull dependencies
include(${PROJECT_SOURCE_DIR}/cmake/testAppSetupProject.cmake)
include(testAppPullDependencies)

# Enable download dependencies only in order to save resources by dowloading all toolchain depencies once
if(TESTAPP_DOWNLOAD_ONLY)
  return()
endif()
if(TESTAPP_ENABLE_TESTS)
  include(testAppAddTest)
endif()


if("${CMAKE_PROJECT_NAME}" STREQUAL "TestApp")
  # Standalone build: for example tests that can be run on Target.
  # Add integration test example to create 'IntegrationTests' Object library for tests to be referenced.
  # when TestApp is built as submodule, 'IntegrationTests' library should be defined in the top project and extended within its context.
  add_subdirectory(${PROJECT_SOURCE_DIR}/IntegrationTests)
endif()
add_subdirectory(${PROJECT_SOURCE_DIR}/Platform)
add_subdirectory(${PROJECT_SOURCE_DIR}/TestApp)
add_subdirectory(${PROJECT_SOURCE_DIR}/OSAdapter/src/Observer)
add_subdirectory(${PROJECT_SOURCE_DIR}/OSAdapter/src/OsAbstraction)
add_subdirectory(${PROJECT_SOURCE_DIR}/OSAdapter/src/OsExtension)

if(DEVICE_UNDER_TEST_ARCH STREQUAL "ARM")
  message(STATUS "DEVICE_UNDER_TEST_ARCH ${DEVICE_UNDER_TEST_ARCH}")
else()
  message(STATUS "DEVICE_UNDER_TEST_ARCH '${DEVICE_UNDER_TEST_ARCH}' is not defined")
endif()

message(STATUS "RUNNING_UNITTESTS_WITH_EMULATOR ${RUNNING_UNITTESTS_WITH_EMULATOR}")
message(STATUS "APP_OSADAPTER ${APP_OSADAPTER}")
message(STATUS "TESTAPP_ENABLE_TESTS ${TESTAPP_ENABLE_TESTS}")
message(STATUS "TESTAPP_ENABLE_MOCKS ${TESTAPP_ENABLE_MOCKS}")
message(STATUS "TESTAPP_STANDALONE_BUILD ${TESTAPP_STANDALONE_BUILD}")
message(STATUS "MCU ${MCU}")

# Add TestApp unit tests
if(TESTAPP_ENABLE_TESTS)
  add_subdirectory(TestApp/test)
endif()

# Add TestApp mocks
if(TESTAPP_ENABLE_MOCKS)
  add_subdirectory(TestApp/mock)
endif()
