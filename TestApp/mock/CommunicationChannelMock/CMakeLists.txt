set(MODULE_NAME "CommunicationChannel-Mock")

add_library(${MODULE_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/CommunicationChannelMock.cpp
  )

target_include_directories(${MODULE_NAME}
  PUBLIC
    ${PROJECT_SOURCE_DIR}/TestApp/mock/CommunicationChannelMock
   # ${CMAKE_SOURCE_DIR}/Platform/RequiredInterface/CommunicationChannelInterface
  )

target_link_libraries(${MODULE_NAME}
  PUBLIC
    gmock
    CommunicationChannel-Interface
    OsExtension
  )
