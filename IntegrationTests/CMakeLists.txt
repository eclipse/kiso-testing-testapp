# In order to define new integration test object library within top project:
# You need to creates an Object Library which compiles source files but does not archive or link their object files into a library. 
# Instead other targets created by add_library() such as TestApp in our case will reference the objects.

add_subdirectory(DemoApplication)

# 1. Add integration tests to object library to be referenced by TestApp
add_library(IntegrationTests OBJECT
  ${CMAKE_CURRENT_SOURCE_DIR}/test_suite/testSuite1.cpp 
)

# 2. Add includes for test dependencies...
target_include_directories(IntegrationTests
  PUBLIC
)

# 3. Specify c++ compile features used in the integartion tests
target_compile_features(IntegrationTests
  PUBLIC
    cxx_std_14
  )

# 4. Add test dependencies...
target_link_libraries(IntegrationTests
  PUBLIC
    Demoapplication
    TestApp-Interface
    TestEnvironment
)
