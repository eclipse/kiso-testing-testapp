/**
 * @file Platform/RequiredInterface/TestAppRtosInterface/TestAppRtosInterface/TaskConfig.hpp
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

#ifndef TESTAPPRTOSINTERFACE_TASKCONFIG_HPP
#define TESTAPPRTOSINTERFACE_TASKCONFIG_HPP

#include <array>
#include <FreeRTOS.h>
#include <task.h>

namespace testapprtos {

#if configENABLE_MPU
/** Expression to define how many custom regions can be defined via RtosTaskUnprivileged::defineCustomRegion */
static constexpr int32_t maxCustomDefinableRegions = 10;

/**
 * This container holds the necessary members
 * that all Task Regions have to define
 * in order to setup the regions access of RAM and
 * exRAM.
 */
struct FixedUsedRegions {
  void* ramBase;   /**< Internal Ram Address of allowed section */
  void* ramEnd;    /**< Maximum internal Ram Address of Task Region */
  void* exRamBase; /**< External Ram Address of allowed section */
  void* exRamEnd;  /**< Maximum external Ram Address of Task Region */
};

/**
 * Enumeration for assigning the task region sections
 * in appropriate order for the rtos Mpu port.
 */
enum class TaskRegionAssignment : uint8_t {
  RegionAssignedRAM,   /**< Region ram section */
  RegionAssignedExRAM, /**< Region exram section */
  UserRegion01,        /**< User definable Region 1 */
  UserRegion02,        /**< User definable Region 2 */
  UserRegion03,        /**< User definable Region 3 */
  UserRegion04,        /**< User definable Region 4 */

  NumberOfElements /**< Elements in this enumeration */
};

/*
 * Helper expressions for
 * portable and easy access to according Sections
 */
/** First User Region */
static constexpr TaskRegionAssignment firstUserRegion = TaskRegionAssignment::UserRegion01;
/** Last User Region */
static constexpr TaskRegionAssignment lastUserRegion = TaskRegionAssignment::UserRegion04;
/** Determine User region Amount */
static constexpr size_t userRegionAmount = static_cast<size_t>(static_cast<uint32_t>(lastUserRegion) - static_cast<uint32_t>(firstUserRegion) + 1u);
/** Expression for Enum comparison */
static constexpr size_t taskRegionAssignment_amount = static_cast<size_t>(TaskRegionAssignment::NumberOfElements);

/**
 * List of all User Definable Regions to
 * grant a task access to.
 */
enum class TaskAccessRegions : uint8_t {
  NoRegionDefined, /**< No User Task Access */
  /* Memories */
  FullExram,            /**< Full Access to all exram elements */
  FullRam,              /**< Full Access to all internal ram elements */
  FullOtp,              /**< Full Access to otp memory */
  FullTestRunner,       /**< Full Access to TestRunner */
  FullTestEnvironment,  /**< Full Access to TestEnvironment */
  Bootloader1Version,   /**< Access to bootloader 1 version section */
  Bootloader2Version,   /**< Access to bootloader 2 version section */
  /* Peripherals */
  FullPeripheralAccess, /**< Full Access to all peripheral elements */
  PeripheralI2c2,       /**< Task Access to Peripheral: I2C2 */
  PeripheralI2c4,       /**< Task Access to Peripheral: I2C4 */
  PeripheralUart,       /**< Task Access to Peripheral: UART */
  PeripheralGpio,       /**< Task Access to Peripheral: All GPIOs */
  PeripheralUsb,        /**< Task Access to Peripheral: USB */
  PeripheralCan,        /**< Task Access to Peripheral: CAN */
  PeripheralHash,       /**< Task Access to Peripheral: Hash Generator */
  PeripheralRng,        /**< Task Access to Peripheral: Random Number Generator */
  PeripheralTimer2,     /**< Task Access to Peripheral: Timer 2 */
  PeripheralOpAmp,      /**< Task Access to Peripheral: Operation Amplifier */
  PeripheralRtc,        /**< Task Access to Peripheral: Real Time Clock */
  PeripheralDma,        /**< Task Access to Peripheral: DMA */
  PeripheralSdmmc,      /**< Task Access to Peripheral: SDMMC */
  PeripheralAes,        /**< Task Access to Peripheral: AES */
  PeripheralFlash,      /**< Task Access to Peripheral: Flash Controller */
  PeripheralICache,     /**< Task Access to Peripheral: ICache */
  PeripheralTamp,       /**< Task Access to Peripheral: TAMP  */
  PeripheralRcc,        /**< Task Access to Peripheral: Reset and Clock Control  */

  NumberOfElements /**< Elements in Enumeration */
};

/** Expression for Enum comparisons */
static constexpr size_t taskAccessRightTypes_amount = static_cast<size_t>(TaskAccessRegions::NumberOfElements);
#endif //configENABLE_MPU

/**
 *  Given Task Definition Parameter by User
 */
struct TaskConfiguration {
  // Settings for Thread Resources
  const uint32_t stackSize;                                     /**< Stack Size in bytes */
  StackType_t * stackPtr;                                        /**< Pointer to the Stack */
  const char* name;                                             /**< Display Name */
  const uint8_t priority;                                       /**< Priority (caution, free rtos priority a.k.a 0 = lowest) */
#if configENABLE_MPU  
  FixedUsedRegions memRegions;                                  /**< @see fixedUsedRegions */
  std::array<TaskAccessRegions, userRegionAmount> regionPermit; /**< @see taskAccessRegions */
#endif
};

}  // namespace testapprtos

#endif  // TESTAPPRTOSINTERFACE_TASKCONFIG_HPP
