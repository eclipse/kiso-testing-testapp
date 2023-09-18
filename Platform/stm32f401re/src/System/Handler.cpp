/**
 * @file Platform/${PROJECT_PLATFORM}/src/System/Handler.cpp
 * @brief Hardfault Handler
 *
 * The Hard Fault Handler will be called if an Hardfault, Usagefault, Memfault, Busfault or an Securefault occurs.
 * If an error happen, the assembler code saves all relevant Register and Stack information onto the MSP Stack.
 *
 * MSP Stack before Fault Handler Call
 * |            |  Register  | Position    |
 * |-----------:|:----------:|:------------|
 * | MSP ->     |   SP       | Element(0)  |
 * |	        |   MSP      | Element(1)  |
 * |		    |   PSP      | Element(2)  |
 * |		    |   R0       | Element(3)  |
 * |		    |   R1       | Element(4)  |
 * |		    |   R2       | Element(5)  |
 * |		    |   R3       | Element(6)  |
 * |		    |   R12      | Element(7)  |
 * |		    |   LR       | Element(8)  |
 * |		    |   PC       | Element(9)  |
 * |		    |   xPSR     | Element(10) |
 * |		    |   R4       | Element(11) |
 * |	        |   R5       | Element(12) |
 * |		    |   R6       | Element(13) |
 * |		    |   R7       | Element(14) |
 * |		    |   R8       | Element(15) |
 * |		    |   R9       | Element(16) |
 * |		    |   R10      | Element(17) |
 * |		    |   R11      | Element(18) |
 * | old_MSP -> |  old Value |             |
 *
 * Then the FaultHandler function will be called to do a stack dump and fault analysis.
 * The function ends with a BKPT instruction to force control back into the
 * debugger or stops in a while loop if no debugger is connected.
 *
  * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * @author Aymen Bouldjadj
 */

// #include <Hal/Cpu.hpp>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <newlib.h>
// #include <Hal/Hal.hpp>
#include <TestAppRtos/Rtos.hpp>
//#include <SEGGER_RTT.h>

/**
 * Implemented Zero Vector that is directly called upon
 * System Reset.
 *
 * It Initializes all RAM Data for all Process Regions
 * and Calls Constructors
 */
extern "C" void resetHandlerC();

/**
 * @brief Application Start Main
 * External Linkage Symbol to main
 */
extern "C" int main();

/**
 * @brief Provided System Start Function
 * External Linkage Symbol Calling the System Clock Bootup
 */
extern "C" void SystemInit();  // NOLINT(readability-identifier-naming) Name defined by third-party library

/**
 * @brief Provided System Start Function for Clock Variable Update
 * External Linkage Symbol Calling the System Clock Update after Init to make sure all Variables are set correct
 */
extern "C" void SystemCoreClockUpdate();  // NOLINT(readability-identifier-naming) Name defined by third-party library

/**
 * @brief Newlib init array
 * External Linkage Symbols calling to newlib
 * members. Used for Constructing
 */
extern "C" void __libc_init_array();  // NOLINT(readability-identifier-naming) Name defined by third-party library

/**
 * @brief Newlib finit array
 * External Linkage Symbols calling to newlib
 * members. Used for Constructing
 */
extern "C" void __libc_fini_array();  // NOLINT(readability-identifier-naming) Name defined by third-party library

/**
 * @brief Reset Handler Provided to ISR vector Table.
 *
 * It Sets up Main Stack Pointer and its Checking Register,
 * then jumps to the C Routine initializing the System Memories.
 */
extern "C" __attribute__((naked)) void Reset_Handler() {  // NOLINT(readability-identifier-naming) Name defined by third-party library
  __asm volatile(
      "ldr   sp, =_estack     \n" /* Import Main-Stack and set Stack Pointer */
      // "ldr   r0, =_sstack     \n" /* Get lowest address of stack region into R0 */
      "adds  r0, #4           \n" /* Increase by 4 (word width) to point to next empty RAM Cell */
      "msr   msplim, r0       \n" /* Set MSP Limit Register */
      "bl    resetHandlerC    \n" /* Branch to Reset Handler that is written in a more sane manner */
  );
}

/**
 * @brief Handles faults from CPU.
 */
extern "C" __attribute__((naked)) void HardFault_Handler() {  // NOLINT(readability-identifier-naming) Name defined by third-party library
  __asm volatile(
      "  tst lr, #4                   \n"  // check if third bit in the link register is set. This check is to figure out if MSP or PSP stack before Fault was active.
      "  ite eq                       \n"
      "  mrseq r0, msp                \n"    // Move MSP Register value into R0
      "  mrsne r0, psp                \n"    // Move PSP Register value into R0
      "  mrs r1, msp                  \n"    // Move MSP Register value into R1
      "  mrs r2, psp                  \n"    // Move PSP Register value into R2
      "  ldr r3, =0xE000ED08          \n"    // Read Vector Table offset Register Address into R3
      "  ldr r3, [r3]                 \n"    // Read Vector Table offset Value in R3
      "  ldr r3, [r3]                 \n"    // Read initial MSP value
      "  msr msp, r3                  \n"    // Move R3 Register value into MSP Register to reinitialize MSP
      "  push {r4-r11}                \n"    // Push R4-R11 Register onto the Stack to save value during exception
      "  ldm r0, {r4-r11}             \n"    // Load the contextswitch data into Register R4-R11 (R0-R3,R12,LR,PC,xPSR this values will be stacked by hardware if a fault happen)
      "  push {r4-r11}                \n"    // Push R4-R11 onto MSP Stack
      "  push {r0-r2}                 \n"    // Push R0-R2 onto MSP
      "  mrs r0, msp                  \n"    // Move MSP into R0
      "  b hardFaultHandlerC          \n");  // Call Fault Handler with Argument R0=MSP
}

/**
  * @brief  FaultHandler C-Function to do a stack dump and fault analysis
  * @param  hardfaultArgs Pointer to Stack for stack dump
  */
extern "C" void hardFaultHandlerC(const uint32_t* hardfaultArgs) {

  __asm("BKPT #0\n");  // cause the debugger to stop

  while (true) {  // stop if no debugger is present
  }
}

/**
 * @brief C Routine for System Reset
 *
 * Loops All Task Regions and sets up all Data and Bss for the system.
 */
extern "C" void resetHandlerC() {
  /* Import of Standard Regions */
  extern uint32_t _sidata[];  // NOLINT(readability-identifier-naming) Name defined by third-party library
  extern uint32_t _sdata[];   // NOLINT(readability-identifier-naming) Name defined by third-party library
  extern uint32_t _edata[];   // NOLINT(readability-identifier-naming) Name defined by third-party library
  extern uint32_t _sbss[];    // NOLINT(readability-identifier-naming) Name defined by third-party library
  extern uint32_t _ebss[];    // NOLINT(readability-identifier-naming) Name defined by third-party library
  /* Import of Privileged Regions */
  // extern uint32_t __privileged_sram_start__[];     // NOLINT(readability-identifier-naming) Name defined by third-party library
  // extern uint32_t __privileged_sram_loadaddr__[];  // NOLINT(readability-identifier-naming) Name defined by third-party library
  // extern uint32_t __privileged_sram_end__[];       // NOLINT(readability-identifier-naming) Name defined by third-party library

  // TODO This file replaces the default asm startup completely. Make sure it is not in the build process.

  /* Start System PLL and Clock for faster initialization */
  // // SystemInit();

  /* Copy Standard Area Data (is common, but just in case) */
  memcpy(_sdata, _sidata, (uint32_t)((uint32_t)_edata - (uint32_t)_sdata));  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast) This type of Cast is absolutely needed at this point, since there is no other way to interpret Linker variables to initialize bss and Data Regions
  memset(_sbss, 0, (uint32_t)((uint32_t)_ebss - (uint32_t)_sbss));           // NOLINT(cppcoreguidelines-pro-type-cstyle-cast) This type of Cast is absolutely needed at this point, since there is no other way to interpret Linker variables to initialize bss and Data Regions
  /* Set Privileged Data */
  // memcpy(__privileged_sram_start__, __privileged_sram_loadaddr__, (uint32_t)((uint32_t)__privileged_sram_end__ - (uint32_t)__privileged_sram_start__));  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast) This type of Cast is absolutely needed at this point, since there is no other way to interpret Linker variables to initialize bss and Data Regions

  /* ------------------------------------------------- User Process Region Initialization  -------------------------------------------------- */

  // /* Copy Task Region Data */
  // INITIALIZE_REGION(TestRunnerRegion);
  
  // INITIALIZE_REGION(TestEnvironmentRegion);

  /* This call must be performed AFTER the Initialization of the memory regions, otherwise the variable gets overwritten with its init values */
  // // SystemCoreClockUpdate();

  /* Initialize Constructors */
  __libc_init_array();
#pragma GCC diagnostic ignored "-Wpedantic"
  main();  // NOLINT(clang-diagnostic-main) This is the Reset Handler. The only place where the call to main is necessary.
#pragma GCC diagnostic warning "-Wpedantic"
  /* Call to destructors */
  __libc_fini_array();

  while (true) {
  };
}
