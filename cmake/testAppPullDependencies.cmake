# Download depedencies (in case we're build Common stand-alone)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  # I am top-level project.
  include(FetchContent)

  set(EXTERNAL_LOCATION ${PROJECT_BINARY_DIR}/external CACHE STRING "Download location for externals")
  set(EXTERNAL_DOWNLOAD ON CACHE STRING "Enable download for externals")
  set(EXTERNAL_DOWNLOAD_ALL OFF CACHE STRING "Enable download of all externals compiler ID independent")

  get_filename_component(EXTERNAL_LOCATION ${EXTERNAL_LOCATION} ABSOLUTE BASE_DIR ${PROJECT_BINARY_DIR})

  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
  set(DOWNLOAD_ARM_EXTERNALS ON CACHE STRING "Enable download for arm externals")
  endif()

  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND (NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "arm"))
    set(DOWNLOAD_GCC_EXTERNALS ON CACHE STRING "Enable download for gcc externals")
  endif()

  if((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
    set(DOWNLOAD_CLANG_EXTERNALS ON CACHE STRING "Enable download for clang externals")
  endif()

  # TODO download all external repositories only once and checkout the required tags into different folders. 
  if(EXTERNAL_DOWNLOAD_ALL)
    set(DOWNLOAD_GCC_EXTERNALS ON CACHE STRING "Enable download for gcc externals")
    set(DOWNLOAD_GHS_EXTERNALS ON CACHE STRING "Enable download for ghs externals")
    set(DOWNLOAD_CLANG_EXTERNALS ON CACHE STRING "Enable download for clang externals")
    set(DOWNLOAD_ARM_EXTERNALS ON CACHE STRING "Enable download for arm externals")
  endif()

  if(NOT EXTERNAL_DOWNLOAD) 
    set(FETCHCONTENT_FULLY_DISCONNECTED ON)

    get_filename_component(FETCHCONTENT_SOURCE_DIR_GOOGLETEST ${EXTERNAL_LOCATION}/googletest-src ABSOLUTE)
    get_filename_component(FETCHCONTENT_SOURCE_DIR_ETL ${EXTERNAL_LOCATION}/etl-src ABSOLUTE)
    get_filename_component(FETCHCONTENT_SOURCE_DIR_FREERTOS_ARM ${EXTERNAL_LOCATION}/arm/FreeRTOS-src ABSOLUTE)
    get_filename_component(FETCHCONTENT_SOURCE_DIR_FREERTOS_GCC ${EXTERNAL_LOCATION}/FreeRTOS-src ABSOLUTE)
    get_filename_component(FETCHCONTENT_SOURCE_DIR_SYSTEMCOMMON ${EXTERNAL_LOCATION}/SystemCommon-src ABSOLUTE)
    get_filename_component(FETCHCONTENT_SOURCE_DIR_STM32F401RE ${EXTERNAL_LOCATION}/stm32cubef4-src ABSOLUTE)
    get_filename_component(FETCHCONTENT_SOURCE_DIR_RTT ${EXTERNAL_LOCATION}/RTT-src ABSOLUTE)

  endif()

  # Googletest
  set(GTEST_ARM_SOURCE_DIR ${EXTERNAL_LOCATION}/arm/googletest-src)
  set(GTEST_ALL_BINARY_DIR ${PROJECT_BINARY_DIR}/external/googletest-build)


  # Googletest for ARM
  if(DOWNLOAD_ARM_EXTERNALS AND RUNNING_UNITTESTS_WITH_EMULATOR)
    FetchContent_Declare(
      googletest_arm_ghs
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG        release-1.12.1
      SOURCE_DIR     ${GTEST_ARM_SOURCE_DIR}
      BINARY_DIR     ${GTEST_ALL_BINARY_DIR}
      SUBBUILD_DIR   ${PROJECT_BINARY_DIR}/external/googletest-subbuild
    )
    FetchContent_GetProperties(googletest_arm_ghs)
    FetchContent_Populate(googletest_arm_ghs)
    add_subdirectory(${GTEST_ARM_SOURCE_DIR} ${GTEST_ALL_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()

  if((DOWNLOAD_GCC_EXTERNALS) OR (DOWNLOAD_CLANG_EXTERNALS))
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG        release-1.12.1 
      SOURCE_DIR     ${EXTERNAL_LOCATION}/googletest-src
      BINARY_DIR     ${GTEST_ALL_BINARY_DIR}
      SUBBUILD_DIR   ${PROJECT_BINARY_DIR}/external/googletest-subbuild
    )
    FetchContent_GetProperties(googletest)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${GTEST_ALL_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()

  # FreeRTOS
  set(FREERTOS_ARM_SOURCE_DIR ${EXTERNAL_LOCATION}/arm/FreeRTOS-src)
  set(FREERTOS_ALL_BINARY_DIR ${PROJECT_BINARY_DIR}/external/FreeRTOS-build)

  if(DOWNLOAD_ARM_EXTERNALS AND NOT RUNNING_UNITTESTS_WITH_EMULATOR)
    # FreeRTOS for ARM
    FetchContent_Declare(
      freertos_arm
      GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS.git
      GIT_TAG        V10.3.0 
      SOURCE_DIR     ${FREERTOS_ARM_SOURCE_DIR}
      BINARY_DIR     ${FREERTOS_ALL_BINARY_DIR}
      SUBBUILD_DIR   ${PROJECT_BINARY_DIR}/external/FreeRTOS-subbuild
    )
  
    FetchContent_GetProperties(freertos_arm)
    FetchContent_Populate(freertos_arm)

    set(FREERTOS_CONF ${PROJECT_SOURCE_DIR}/Platform/stm32f401re/config/FreeRTOS)
    set(FREERTOS_PORT GCC/ARM_CM4F)
    #set(FREERTOS_LINK_LIBRARIES)
    set(FREERTOS_PORT_SOURCES port.c)
  endif()

  if(DOWNLOAD_GCC_EXTERNALS OR DOWNLOAD_CLANG_EXTERNALS OR RUNNING_UNITTESTS_WITH_EMULATOR)
    # FreeRTOS from SystemCommon
    FetchContent_Declare(
      freertos_gcc
      GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS.git
      GIT_TAG        V10.3.0 
      SOURCE_DIR     ${EXTERNAL_LOCATION}/FreeRTOS-src
      BINARY_DIR     ${FREERTOS_ALL_BINARY_DIR}
      SUBBUILD_DIR   ${PROJECT_BINARY_DIR}/external/FreeRTOS-subbuild
    )

    FetchContent_GetProperties(freertos_gcc)
    FetchContent_Populate(freertos_gcc)

    set(FREERTOS_CONF ${PROJECT_SOURCE_DIR}/Platform/hostMachine-gcc/config/FreeRTOS)
    set(FREERTOS_PORT NullPort)
    set(FREERTOS_LINK_LIBRARIES)
    set(FREERTOS_PORT_SOURCES)
  endif()
    
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
    
    add_subdirectory(${PROJECT_SOURCE_DIR}/external/FreeRTOS ${FREERTOS_ALL_BINARY_DIR} EXCLUDE_FROM_ALL)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GHS")
  #  add_subdirectory(${freertos_ghs_SOURCE_DIR} ${FREERTOS_ALL_BINARY_DIR} EXCLUDE_FROM_ALL)   #TODO cleanup PPC Platform (freeRtos should be pulled and configured as an external dependency)
  else()  
    add_subdirectory(${PROJECT_SOURCE_DIR}/external/FreeRTOS ${FREERTOS_ALL_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
  
  # stm32cubef4
  if(DOWNLOAD_ARM_EXTERNALS AND MCU STREQUAL "STM32F401RE")
  # stm32cubef4
  FetchContent_Declare(
    stm32cubef4 
    GIT_REPOSITORY https://github.com/STMicroelectronics/STM32CubeF4.git
    GIT_TAG        v1.27.1
    SOURCE_DIR     ${EXTERNAL_LOCATION}/stm32cubef4-src
    BINARY_DIR     ${PROJECT_BINARY_DIR}/external/stm32cubef4-build
    SUBBUILD_DIR   ${PROJECT_BINARY_DIR}/external/stm32cubef4-subbuild
  )
  FetchContent_GetProperties(stm32cubef4)
  FetchContent_Populate(stm32cubef4)
  endif()

  if(MCU STREQUAL "STM32F401RE")
    configure_file(${PROJECT_SOURCE_DIR}/Platform/stm32f401re/config/STM32CubeF4/cmake/hal.cmake ${PROJECT_BINARY_DIR}/external/stm32cubef4-src/cmake/hal.cmake)
    configure_file(${PROJECT_SOURCE_DIR}/Platform/stm32f401re/config/STM32CubeF4/cmake/cmsis.cmake ${PROJECT_BINARY_DIR}/external/stm32cubef4-src/cmake/cmsis.cmake)
    configure_file(${PROJECT_SOURCE_DIR}/Platform/stm32f401re/config/STM32CubeF4/cmake/usb-device.cmake ${PROJECT_BINARY_DIR}/external/stm32cubef4-src/cmake/usb-device.cmake)
    configure_file(${PROJECT_SOURCE_DIR}/Platform/stm32f401re/config/STM32CubeF4/cmake/CMakeLists.txt ${PROJECT_BINARY_DIR}/external/stm32cubef4-src/CMakeLists.txt)

    set(stm32cubef4_DEVICE ${MCU})
    set(stm32cubef4_CONF ${PROJECT_SOURCE_DIR}/Platform/stm32cubef4/config/stm32cubeF4)
    # workaround to move doxygen.cmake file to the path referred to by tps "stm32cubef4". 
    # TODO: decide whether if an stm32cubef4 tag (with the corresponding new path) should be created, create an external folder and add the file, or use the following to move it there at build time.
    # configure_file(${toolchain_SOURCE_DIR}/cmake/config/doxygen.cmake ${PROJECT_SOURCE_DIR}/external/Toolchain/cmake/config/doxygen.cmake)
    
    add_subdirectory(${stm32cubef4_SOURCE_DIR} ${stm32cubef4_BINARY_DIR} EXCLUDE_FROM_ALL)

  endif()

  # rtt is not pulled is still a dependency
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
    set(RTT_CONF ${PROJECT_SOURCE_DIR}/Platform/stm32f401re/config/RTT)
    add_subdirectory(${PROJECT_SOURCE_DIR}/external/Segger-RTT  EXCLUDE_FROM_ALL)
  endif()

  FetchContent_Declare(
    etl
    GIT_REPOSITORY https://github.com/ETLCPP/etl
    GIT_TAG        18.1.3 
    SOURCE_DIR     ${EXTERNAL_LOCATION}/etl-src
    BINARY_DIR     ${PROJECT_BINARY_DIR}/external/etl-build
    SUBBUILD_DIR   ${PROJECT_BINARY_DIR}/external/etl-subbuild
  )
  FetchContent_GetProperties(etl)
  FetchContent_Populate(etl)

  add_subdirectory(${etl_SOURCE_DIR} ${etl_BINARY_DIR} EXCLUDE_FROM_ALL)
  target_compile_definitions(etl INTERFACE -DETL_ARRAY_VIEW_IS_MUTABLE)

  # etl config to be provided by target-platform  
  target_include_directories(etl INTERFACE ${PROJECT_SOURCE_DIR}/Platform/stm32f401re/config/etl) 
endif()
