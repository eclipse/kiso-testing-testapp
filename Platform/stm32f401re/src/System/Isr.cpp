/**
 * @file src/Core/Isr.cpp
 * @brief Interrupt Vector Table
 *
 * The Interrupt Vector Table is linked at a specific Address to map
 * occuring ISRs onto the right Functions.
 * The Naming Conventions are suppressed here, since the standard 
 * CMSIS Vector names are used.
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

#include <cstdint>

/**
 * Import the top of Stack provided by linker
 */
extern uint32_t _estack;  // NOLINT(readability-identifier-naming) Linker defined standard variable.

/** ISR Vector definition */
using IsrVectorEntry = void (*)();

struct IsrVectorTable {
  uint32_t* topOfStack;                  /**< Stack Pointer is Vector Table First entry by definition */
  IsrVectorEntry resetHandler;           /**< Member Correlating to an ISR Vector */
  IsrVectorEntry nmiHandler;             /**< Member Correlating to an ISR Vector */
  IsrVectorEntry hardFaultHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry memManageHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry busFaultHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry usageFaultHandler;      /**< Member Correlating to an ISR Vector */
  IsrVectorEntry secureFaultHandler;     /**< Member Correlating to an ISR Vector */
  IsrVectorEntry reserved01;             /**< Member Correlating to an ISR Vector */
  IsrVectorEntry reserved02;             /**< Member Correlating to an ISR Vector */
  IsrVectorEntry reserved03;             /**< Member Correlating to an ISR Vector */
  IsrVectorEntry svcHandler;             /**< Member Correlating to an ISR Vector */
  IsrVectorEntry debugMonHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry reserved04;             /**< Member Correlating to an ISR Vector */
  IsrVectorEntry pendSvHandler;          /**< Member Correlating to an ISR Vector */
  IsrVectorEntry sysTickHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry wwdgIrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry pdvPvmIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry rtcIrqHandler;          /**< Member Correlating to an ISR Vector */
  IsrVectorEntry rtcSIrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tampIrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tampSIrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry flashIrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry flashSIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry gtzcIrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry rccIrqHandler;          /**< Member Correlating to an ISR Vector */
  IsrVectorEntry rccSIrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti0IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti1IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti2IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti3IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti4IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti5IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti6IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti7IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti8IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti9IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti10IrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti11IrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti12IrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti13IrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti14IrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry exti15IrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dmaMux1IrqHandler;      /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dmaMux1SIrqHandler;     /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma1Channel1IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma1Channel2IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma1Channel3IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma1Channel4IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma1Channel5IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma1Channel6IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma1Channel7IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma1Channel8IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry adc12IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dacIrqHandler;          /**< Member Correlating to an ISR Vector */
  IsrVectorEntry fdcan1It0IrqHandler;    /**< Member Correlating to an ISR Vector */
  IsrVectorEntry fdcan1It1IrqHandler;    /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim1BrkIrqHandler;      /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim1UpIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim1TrgComIrqHandler;   /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim1CcIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim2IrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim3IrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim4IrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim5IrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim6IrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim7IrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim8BrkIrqHandler;      /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim8UpIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim8TrgComIrqHandler;   /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim8CcIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry i2c1EvIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry i2c1ErIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry i2c2EvIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry i2c2ErIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry spi1IrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry spi2IrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry usart1IrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry usart2IrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry usart3IrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry uart4IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry uart5IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry lpuart1IrqHandler;      /**< Member Correlating to an ISR Vector */
  IsrVectorEntry lptim1IrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry lptim2IrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim15IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim16IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tim17IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry compIrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry usbFsIrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry crsIrqHandler;          /**< Member Correlating to an ISR Vector */
  IsrVectorEntry fmcIrqHandler;          /**< Member Correlating to an ISR Vector */
  IsrVectorEntry octospi1IrqHandler;     /**< Member Correlating to an ISR Vector */
  IsrVectorEntry reserved05;             /**< Member Correlating to an ISR Vector */
  IsrVectorEntry sdmmc1IrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry reserved06;             /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma2Channel1IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma2Channel2IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma2Channel3IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma2Channel4IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma2Channel5IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma2Channel6IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma2Channel7IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dma2Channel8IrqHandler; /**< Member Correlating to an ISR Vector */
  IsrVectorEntry i2c3EvIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry i2c3ErIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry sai1IrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry sai2IrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry tscIrqHandler;          /**< Member Correlating to an ISR Vector */
  IsrVectorEntry aesIrqHandler;          /**< Member Correlating to an ISR Vector */
  IsrVectorEntry rngIrqHandler;          /**< Member Correlating to an ISR Vector */
  IsrVectorEntry fpuIrqHandler;          /**< Member Correlating to an ISR Vector */
  IsrVectorEntry hashIrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry pkaIrqHandler;          /**< Member Correlating to an ISR Vector */
  IsrVectorEntry lptim3IrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry spi3IrqHandler;         /**< Member Correlating to an ISR Vector */
  IsrVectorEntry i2c4ErIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry i2c4EvIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dfsdm1Flt0IrqHandler;   /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dfsdm1Flt1IrqHandler;   /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dfsdm1Flt2IrqHandler;   /**< Member Correlating to an ISR Vector */
  IsrVectorEntry dfsdm1Flt3IrqHandler;   /**< Member Correlating to an ISR Vector */
  IsrVectorEntry ucpd1IrqHandler;        /**< Member Correlating to an ISR Vector */
  IsrVectorEntry icacheIrqHandler;       /**< Member Correlating to an ISR Vector */
  IsrVectorEntry otfdec1IrqHandler;      /**< Member Correlating to an ISR Vector */
};
/** Reassign Type of vector Table */
using IsrVectorTable_t = struct IsrVectorTable;  // NOLINT(readability-identifier-naming) Readability is better this way.

/**
 * @brief 'Generic' C handler for faults
 *
 * This Handler is a Fallback if a unhandled and/or unimplemented
 * ISR is called.
 * This handler jumps straight to Hard Fault for Stack unwinding.
 */
extern "C" void HandlerC();  // NOLINT(readability-identifier-naming) Name defined by third-party library

/** @brief Import of the System Reset Handler */
extern "C" __attribute__((naked)) void Reset_Handler();  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of the System Hard Fault Handler */
extern "C" __attribute__((naked)) void HardFault_Handler();  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void NMI_Handler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void MemManage_Handler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void BusFault_Handler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void UsageFault_Handler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void SecureFault_Handler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void SVC_Handler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DebugMon_Handler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void PendSV_Handler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void SysTick_Handler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void WWDG_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void PVD_PVM_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void RTC_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void RTC_S_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TAMP_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TAMP_S_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void FLASH_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void FLASH_S_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void GTZC_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void RCC_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void RCC_S_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI0_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI2_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI3_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI4_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI5_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI6_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI7_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI8_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI9_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI10_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI11_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI12_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI13_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI14_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void EXTI15_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMAMUX1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMAMUX1_S_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA1_Channel1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA1_Channel2_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA1_Channel3_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA1_Channel4_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA1_Channel5_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA1_Channel6_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA1_Channel7_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA1_Channel8_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void ADC1_2_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DAC_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void FDCAN1_IT0_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void FDCAN1_IT1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM1_BRK_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM1_UP_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM1_TRG_COM_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM1_CC_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM2_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM3_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM4_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM5_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM6_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM7_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM8_BRK_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM8_UP_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM8_TRG_COM_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM8_CC_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void I2C1_EV_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void I2C1_ER_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void I2C2_EV_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void I2C2_ER_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void SPI1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void SPI2_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void USART1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void USART2_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void USART3_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void UART4_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void UART5_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void LPUART1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void LPTIM1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void LPTIM2_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM15_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM16_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TIM17_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void COMP_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void USB_FS_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void CRS_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void FMC_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void OCTOSPI1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void SDMMC1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA2_Channel1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA2_Channel2_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA2_Channel3_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA2_Channel4_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA2_Channel5_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA2_Channel6_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA2_Channel7_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DMA2_Channel8_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void I2C3_EV_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void I2C3_ER_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void SAI1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void SAI2_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void TSC_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void AES_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void RNG_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void FPU_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void HASH_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void PKA_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void LPTIM3_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void SPI3_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void I2C4_ER_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void I2C4_EV_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DFSDM1_FLT0_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DFSDM1_FLT1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DFSDM1_FLT2_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void DFSDM1_FLT3_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void UCPD1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void ICACHE_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library
/** @brief Import of Standard ISR Vector */
extern "C" void OTFDEC1_IRQHandler() __attribute__((weak, alias("HandlerC")));  // NOLINT(readability-identifier-naming) Name defined by third-party library

extern "C" void HandlerC() {  // NOLINT(readability-identifier-naming) Name defined by third-party library
  HardFault_Handler();        /* Jump to hard fault handler for unhandled Interrupts */
}

/**
 * @brief ISR Vector Table.
 * Contains all ISR Vectors for the system.
 * Is used to dispatch any Interrupt Request.
 * It was implemented to use the CMSIS Standard names
 * for all Vectors.
 */
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init) estack is a constant - Linker defined.
__attribute__((used)) __attribute__((section(".isr_vector"))) static const IsrVectorTable_t isrVectorTable = {
    // TODO ISR Grouped in separate file
    &_estack,                 /**< topOfStack */
    Reset_Handler,            /**< resetHandler */
    NMI_Handler,              /**< nmiHandler */
    HardFault_Handler,        /**< hardFaultHandler */
    MemManage_Handler,        /**< memManageHandler */
    BusFault_Handler,         /**< busFaultHandler */
    UsageFault_Handler,       /**< usageFaultHandler */
    SecureFault_Handler,      /**< secureFaultHandler */
    nullptr,                  /**< reserved01 */
    nullptr,                  /**< reserved02 */
    nullptr,                  /**< reserved03 */
    SVC_Handler,              /**< svcHandler */
    DebugMon_Handler,         /**< debugMonHandler */
    nullptr,                  /**< reserved04 */
    PendSV_Handler,           /**< pendSvHandler */
    SysTick_Handler,          /**< sysTickHandler */
    WWDG_IRQHandler,          /**< wwdgIrqHandler */
    PVD_PVM_IRQHandler,       /**< pdvPvmIrqHandler */
    RTC_IRQHandler,           /**< rtcIrqHandler */
    RTC_S_IRQHandler,         /**< rtcSIrqHandler */
    TAMP_IRQHandler,          /**< tampIrqHandler */
    TAMP_S_IRQHandler,        /**< tampSIrqHandler */
    FLASH_IRQHandler,         /**< flashIrqHandler */
    FLASH_S_IRQHandler,       /**< flashSIrqHandler */
    GTZC_IRQHandler,          /**< gtzcIrqHandler */
    RCC_IRQHandler,           /**< rccIrqHandler */
    RCC_S_IRQHandler,         /**< rccSIrqHandler */
    EXTI0_IRQHandler,         /**< exti0IrqHandler */
    EXTI1_IRQHandler,         /**< exti1IrqHandler */
    EXTI2_IRQHandler,         /**< exti2IrqHandler */
    EXTI3_IRQHandler,         /**< exti3IrqHandler */
    EXTI4_IRQHandler,         /**< exti4IrqHandler */
    EXTI5_IRQHandler,         /**< exti5IrqHandler */
    EXTI6_IRQHandler,         /**< exti6IrqHandler */
    EXTI7_IRQHandler,         /**< exti7IrqHandler */
    EXTI8_IRQHandler,         /**< exti8IrqHandler */
    EXTI9_IRQHandler,         /**< exti9IrqHandler */
    EXTI10_IRQHandler,        /**< exti10IrqHandler */
    EXTI11_IRQHandler,        /**< exti11IrqHandler */
    EXTI12_IRQHandler,        /**< exti12IrqHandler */
    EXTI13_IRQHandler,        /**< exti13IrqHandler */
    EXTI14_IRQHandler,        /**< exti14IrqHandler */
    EXTI15_IRQHandler,        /**< exti15IrqHandler */
    DMAMUX1_IRQHandler,       /**< dmaMux1IrqHandler */
    DMAMUX1_S_IRQHandler,     /**< dmaMux1SIrqHandler; */
    DMA1_Channel1_IRQHandler, /**< dma1Channel1IrqHandler */
    DMA1_Channel2_IRQHandler, /**< dma1Channel2IrqHandler */
    DMA1_Channel3_IRQHandler, /**< dma1Channel3IrqHandler */
    DMA1_Channel4_IRQHandler, /**< dma1Channel4IrqHandler */
    DMA1_Channel5_IRQHandler, /**< dma1Channel5IrqHandler */
    DMA1_Channel6_IRQHandler, /**< dma1Channel6IrqHandler */
    DMA1_Channel7_IRQHandler, /**< dma1Channel7IrqHandler */
    DMA1_Channel8_IRQHandler, /**< dma1Channel8IrqHandler */
    ADC1_2_IRQHandler,        /**< adc12IrqHandler */
    DAC_IRQHandler,           /**< dacIrqHandler */
    FDCAN1_IT0_IRQHandler,    /**< fdcan1It0IrqHandler */
    FDCAN1_IT1_IRQHandler,    /**< fdcan1It1IrqHandler */
    TIM1_BRK_IRQHandler,      /**< tim1BrkIrqHandler */
    TIM1_UP_IRQHandler,       /**< tim1UpIrqHandler */
    TIM1_TRG_COM_IRQHandler,  /**< tim1TrgComIrqHandler */
    TIM1_CC_IRQHandler,       /**< tim1CcIrqHandler */
    TIM2_IRQHandler,          /**< tim2IrqHandler */
    TIM3_IRQHandler,          /**< tim3IrqHandler */
    TIM4_IRQHandler,          /**< tim4IrqHandler */
    TIM5_IRQHandler,          /**< tim5IrqHandler */
    TIM6_IRQHandler,          /**< tim6IrqHandler */
    TIM7_IRQHandler,          /**< tim7IrqHandler */
    TIM8_BRK_IRQHandler,      /**< tim8BrkIrqHandler */
    TIM8_UP_IRQHandler,       /**< tim8UpIrqHandler */
    TIM8_TRG_COM_IRQHandler,  /**< tim8TrgComIrqHandler */
    TIM8_CC_IRQHandler,       /**< tim8CcIrqHandler */
    I2C1_EV_IRQHandler,       /**< i2c1EvIrqHandler */
    I2C1_ER_IRQHandler,       /**< i2c1ErIrqHandler */
    I2C2_EV_IRQHandler,       /**< i2c2EvIrqHandler */
    I2C2_ER_IRQHandler,       /**< i2c2ErIrqHandler */
    SPI1_IRQHandler,          /**< spi1IrqHandler */
    SPI2_IRQHandler,          /**< spi2IrqHandler */
    USART1_IRQHandler,        /**< usart1IrqHandler */
    USART2_IRQHandler,        /**< usart2IrqHandler */
    USART3_IRQHandler,        /**< usart3IrqHandler */
    UART4_IRQHandler,         /**< uart4IrqHandler */
    UART5_IRQHandler,         /**< uart5IrqHandler */
    LPUART1_IRQHandler,       /**< lpuart1IrqHandler */
    LPTIM1_IRQHandler,        /**< lptim1IrqHandler */
    LPTIM2_IRQHandler,        /**< lptim2IrqHandler */
    TIM15_IRQHandler,         /**< tim15IrqHandler */
    TIM16_IRQHandler,         /**< tim16IrqHandler */
    TIM17_IRQHandler,         /**< tim17IrqHandler */
    COMP_IRQHandler,          /**< compIrqHandler */
    USB_FS_IRQHandler,        /**< usbFsIrqHandler */
    CRS_IRQHandler,           /**< crsIrqHandler */
    FMC_IRQHandler,           /**< fmcIrqHandler */
    OCTOSPI1_IRQHandler,      /**< octospi1IrqHandler */
    nullptr,                  /**< reserved05 */
    SDMMC1_IRQHandler,        /**< sdmmc1IrqHandler */
    nullptr,                  /**< reserved06 */
    DMA2_Channel1_IRQHandler, /**< dma2Channel1IrqHandler */
    DMA2_Channel2_IRQHandler, /**< dma2Channel2IrqHandler */
    DMA2_Channel3_IRQHandler, /**< dma2Channel3IrqHandler */
    DMA2_Channel4_IRQHandler, /**< dma2Channel4IrqHandler */
    DMA2_Channel5_IRQHandler, /**< dma2Channel5IrqHandler */
    DMA2_Channel6_IRQHandler, /**< dma2Channel6IrqHandler */
    DMA2_Channel7_IRQHandler, /**< dma2Channel7IrqHandler */
    DMA2_Channel8_IRQHandler, /**< dma2Channel8IrqHandler */
    I2C3_EV_IRQHandler,       /**< i2c3EvIrqHandler */
    I2C3_ER_IRQHandler,       /**< i2c3ErIrqHandler */
    SAI1_IRQHandler,          /**< sai1IrqHandler */
    SAI2_IRQHandler,          /**< sai2IrqHandler */
    TSC_IRQHandler,           /**< tscIrqHandler */
    AES_IRQHandler,           /**< aesIrqHandler */
    RNG_IRQHandler,           /**< rngIrqHandler */
    FPU_IRQHandler,           /**< fpuIrqHandler */
    HASH_IRQHandler,          /**< hashIrqHandler */
    PKA_IRQHandler,           /**< pkaIrqHandler */
    LPTIM3_IRQHandler,        /**< lptim3IrqHandler */
    SPI3_IRQHandler,          /**< spi3IrqHandler */
    I2C4_ER_IRQHandler,       /**< i2c4ErIrqHandler */
    I2C4_EV_IRQHandler,       /**< i2c4EvIrqHandler */
    DFSDM1_FLT0_IRQHandler,   /**< dfsdm1Flt0IrqHandler */
    DFSDM1_FLT1_IRQHandler,   /**< dfsdm1Flt1IrqHandler */
    DFSDM1_FLT2_IRQHandler,   /**< dfsdm1Flt2IrqHandler */
    DFSDM1_FLT3_IRQHandler,   /**< dfsdm1Flt3IrqHandler */
    UCPD1_IRQHandler,         /**< ucpd1IrqHandler */
    ICACHE_IRQHandler,        /**< icacheIrqHandler */
    OTFDEC1_IRQHandler,       /**< otfdec1IrqHandler */
};
