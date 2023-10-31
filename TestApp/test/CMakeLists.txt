# Include unittests only when:
# 1. using the emulator toolchains (unittest-ppc-ghs-emu & unittest-stm32-arm-gcc) 
# 2. using gcc toolchain for a Native build
if( NOT(RUNNING_UNITTESTS_WITH_EMULATOR STREQUAL "OFF" AND DEVICE_UNDER_TEST_ARCH MATCHES "ARM|PPC") )
  add_subdirectory(Unit)
endif()

