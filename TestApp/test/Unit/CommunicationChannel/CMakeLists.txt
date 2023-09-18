if(DEVICE_UNDER_TEST_ARCH STREQUAL "ARM|PPC")
set(MODULE_NAME "CommunicationChannel")

add_executable(${MODULE_NAME}-Test
  ${MODULE_NAME}Test.cpp
  )

target_include_directories(${MODULE_NAME}-Test 
  PUBLIC
  ${PROJECT_SOURCE_DIR}/Platform/RequiredInterface/CommunicationChannelInterface
  )

target_link_libraries(${MODULE_NAME}-Test
  PUBLIC
    CommunicationChannel
    gtest  
    gmock_main
  )

# Adds test executable as well target to run the test and generate test results
testapp_add_test(
  MODULE_NAME ${MODULE_NAME}
  )

endif()