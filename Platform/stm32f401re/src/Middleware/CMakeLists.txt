add_library(CommunicationChannel
  ${CMAKE_CURRENT_SOURCE_DIR}/RttChannel.cpp
)

target_compile_features(CommunicationChannel
  PUBLIC
    cxx_std_14
  )

target_include_directories(CommunicationChannel
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(CommunicationChannel
  PUBLIC
    CommunicationChannel-Interface
    OsExtension
    rtt
  )
