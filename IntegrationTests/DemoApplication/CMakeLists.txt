# Interface used to write the test
add_library(DemoappTypes
  INTERFACE
  )

target_include_directories(DemoappTypes
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DemoappTypes
  )

target_link_libraries(DemoappTypes
  INTERFACE
    etl
  )

add_library(Tier1competetion
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Tier1competetion/Tier1competetion.cpp
  )
target_include_directories(Tier1competetion
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Tier1competetion
  )
# Specify c++ compile features used in the Tier1competetion
target_compile_features(Tier1competetion
  PUBLIC
    cxx_std_14
  )
# Tier1competetion dependencies
target_link_libraries(Tier1competetion
  PUBLIC
    DemoappTypes
    OsExtension
    etl
  )

add_library(Dashboard
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Dashboard/Dashboard.cpp
  )
target_include_directories(Dashboard
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Dashboard
  )
# Specify c++ compile features used in the Dashboard
target_compile_features(Dashboard
  PUBLIC
    cxx_std_14
  )
# Dashboard dependencies
target_link_libraries(Dashboard
  PUBLIC
    DemoappTypes
    OsExtension
    etl
  )

add_library(Tier2competetion
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Tier2competetion/Tier2competetion.cpp
  )
target_include_directories(Tier2competetion
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Tier2competetion
  )
# Specify c++ compile features used in the Dashboard
target_compile_features(Tier2competetion
  PUBLIC
    cxx_std_14
  )
# Tier2competetion dependencies
target_link_libraries(Tier2competetion
  PUBLIC
    DemoappTypes
    OsExtension
    etl
  )
  
if(DEVICE_UNDER_TEST_ARCH MATCHES "PPC|ARM" AND NOT RUNNING_UNITTESTS_WITH_EMULATOR)
  # TestApp Library (to link within the tests and target platform)
  add_library(Demoapplication
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Dashboard/Task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Dashboard/Dashboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Tier1competetion/Task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Tier1competetion/Tier1competetion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Tier2competetion/Task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Tier2competetion/Tier2competetion.cpp
    )

  target_include_directories(Demoapplication
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/include/DemoappTypes
      ${CMAKE_CURRENT_SOURCE_DIR}/include/Dashboard
      ${CMAKE_CURRENT_SOURCE_DIR}/include/Tier1competetion
      ${CMAKE_CURRENT_SOURCE_DIR}/include/Tier2competetion
      ${PROJECT_SOURCE_DIR}/Platform/RequiredInterface/TestAppRtosInterface
      ${PROJECT_SOURCE_DIR}/Platform/RequiredInterface/CommunicationChannelInterface
    )

  # Specify c++ compile features used in the TestApp
  target_compile_features(Demoapplication
    PUBLIC
      cxx_std_14
    )

  # TestApp dependencies
  target_link_libraries(Demoapplication
    PUBLIC
      TestAppRtos
      etl
    )

endif()


