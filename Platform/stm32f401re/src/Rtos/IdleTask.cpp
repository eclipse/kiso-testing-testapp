/**
 * @file Platform/stm32f401re/src/Rtos/IdleTask.cpp
 * @brief Idle Task Related Callbacks and static memory allocation.
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

#include <TestAppRtos/Rtos.hpp>

#if (configSUPPORT_STATIC_ALLOCATION == 1) && (configUSE_TIMERS == 1)

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so
 the application must provide an implementation of
 vApplicationGetTimerTaskMemory() to provide the memory that is used by the Timer
 service task. */

static constexpr size_t timer_task_stack_size = configTIMER_TASK_STACK_DEPTH;
static StackType_t timer_task_stack[timer_task_stack_size];
static StaticTask_t timer_task_buffer;

extern "C" void vApplicationGetTimerTaskMemory(
    StaticTask_t** ppxTimerTaskTCBBuffer, StackType_t** ppxTimerTaskStackBuffer,
    uint32_t* pulTimerTaskStackSize) {
  *ppxTimerTaskTCBBuffer = &timer_task_buffer;
  *ppxTimerTaskStackBuffer = timer_task_stack;
  *pulTimerTaskStackSize = timer_task_stack_size;
}
#endif
