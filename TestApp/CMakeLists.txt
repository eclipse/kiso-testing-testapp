# Interface used to write the test
add_library(TestApp-Interface
  INTERFACE
  )

target_include_directories(TestApp-Interface
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TestCase
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TestEnvironment
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TestSuite
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TestRegistry
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TestFactory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TestAppTypes
  )

target_link_libraries(TestApp-Interface
  INTERFACE
    etl
  )

# MessageHandler used to define common message Type and methods to handle messages
add_library(MessageHandler
  ${CMAKE_CURRENT_SOURCE_DIR}/src/MessageHandler/MessageHandler.cpp
  )

target_include_directories(MessageHandler
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/MessageHandler
  )

target_link_libraries(MessageHandler
    TestApp-Interface
  )

if(DEVICE_UNDER_TEST_ARCH MATCHES "PPC|ARM" AND NOT RUNNING_UNITTESTS_WITH_EMULATOR)
  # TestApp Library (to link within the tests and target platform)
  add_library(TestApp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TestRunner/Task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TestRunner/TestRunner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TestEnvironment/Assert.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TestEnvironment/AssertReport.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TestEnvironment/Logging.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TestEnvironment/Task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TestEnvironment/TestEnvironment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TPC/Task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TPC/TPC.cpp
    )

  target_include_directories(TestApp
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include/TestRunner
      ${CMAKE_CURRENT_SOURCE_DIR}/include/TestEnvironment
      ${CMAKE_CURRENT_SOURCE_DIR}/include/TPC
      ${PROJECT_SOURCE_DIR}/Platform/RequiredInterface/TestAppRtosInterface
      ${PROJECT_SOURCE_DIR}/Platform/RequiredInterface/CommunicationChannelInterface
    )

  # Specify c++ compile features used in the TestApp
  target_compile_features(TestApp
    PUBLIC
      cxx_std_14
    )

  # TestApp dependencies
  target_link_libraries(TestApp
    PRIVATE
      TestApp-Interface
    PUBLIC
      MessageHandler
      # Reference test objects
      $<TARGET_OBJECTS:IntegrationTests>
      # We need communication channel to be configured/implemented
      CommunicationChannel
      # We need RTOS implementation for the specific target 
      TestAppRtos
      # Third Party Dependencies for TestApp
      etl
    )
endif()

add_library(TestEnvironment
  ${CMAKE_CURRENT_SOURCE_DIR}/src/TestEnvironment/Assert.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/TestEnvironment/AssertReport.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/TestEnvironment/Logging.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/TestEnvironment/TestEnvironment.cpp
  )
target_include_directories(TestEnvironment
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TestEnvironment
  )
# Specify c++ compile features used in the TestEnvironment
target_compile_features(TestEnvironment
  PUBLIC
    cxx_std_14
  )
# TestEnvironment dependencies
target_link_libraries(TestEnvironment
  PRIVATE
    TestApp-Interface
  PUBLIC
    OsExtension
    etl
    MessageHandler
  )

add_library(TestRunner
  ${CMAKE_CURRENT_SOURCE_DIR}/src/TestRunner/TestRunner.cpp
  )
target_include_directories(TestRunner
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TestRunner
  )
# Specify c++ compile features used in the TestRunner
target_compile_features(TestRunner
  PUBLIC
    cxx_std_14
  )
# TestRunner dependencies
target_link_libraries(TestRunner
  PRIVATE
    TestApp-Interface
  PUBLIC
    OsExtension
    etl
    MessageHandler
  )

add_library(TPC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/TPC/TPC.cpp
  )
target_include_directories(TPC
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TPC
  )
# Specify c++ compile features used in the TestRunner
target_compile_features(TPC
  PUBLIC
    cxx_std_14
  )
# TPC dependencies
target_link_libraries(TPC
  PRIVATE
    TestApp-Interface
  PUBLIC
    OsExtension
    etl
    MessageHandler
    CommunicationChannel-Interface
  )
