set(MODULE_NAME "MessageBufferReference-Mock")

add_library(${MODULE_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageBufferReferenceMock.cpp
  )

target_include_directories(${MODULE_NAME}
  PUBLIC
    ${PROJECT_SOURCE_DIR}/TestApp/mock/MessageBufferMock
  )

target_link_libraries(${MODULE_NAME}
  PUBLIC
    OsExtension-Interface
    gmock
  )
