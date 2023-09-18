add_library(TestAppRtos
  IdleTask.cpp
  Rtos.cpp
  )

target_include_directories(TestAppRtos
  PUBLIC
    ${PROJECT_SOURCE_DIR}/Platform/${PROJECT_PLATFORM}/include/Rtos
  )

target_compile_features(TestAppRtos
  PUBLIC
    cxx_std_14
  )
  
target_link_libraries(TestAppRtos
  PUBLIC
  	freertos
  	OsExtension
    OsAbstraction
    TestAppRtos-Interface
  PRIVATE
    #Hal
    #Logger
    #PlatformMemoryDefinitions
	  #Led
  )


