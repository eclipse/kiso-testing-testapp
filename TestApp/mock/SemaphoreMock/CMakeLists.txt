set(MODULE_NAME "SemaphoreMockMock")

add_library(${MODULE_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/SemaphoreMock.cpp
  )

target_include_directories(${MODULE_NAME}
  PUBLIC
    ${PROJECT_SOURCE_DIR}/TestApp/mock/SemaphoreMock
  )

target_link_libraries(${MODULE_NAME}
  PUBLIC
    OsExtension-Interface
    gmock
  )
