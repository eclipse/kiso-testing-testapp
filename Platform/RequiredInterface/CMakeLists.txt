add_library(CommunicationChannel-Interface
  INTERFACE
  )

target_include_directories(CommunicationChannel-Interface
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/CommunicationChannelInterface
  )

add_library(TestAppRtos-Interface
  INTERFACE
  )

target_include_directories(TestAppRtos-Interface
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/TestAppRtosInterface
  )

if(TESTAPP_STANDALONE_BUILD)
  target_compile_definitions(TestAppRtos-Interface
    INTERFACE
      TESTAPP_STANDALONE_BUILD=1
    )
endif()
