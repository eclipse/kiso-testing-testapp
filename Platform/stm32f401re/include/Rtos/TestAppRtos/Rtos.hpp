/**
 * @file include/Rtos/Rtos/Rtos.hpp
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

#ifndef TESTAPPRTOS_HPP
#define TESTAPPRTOS_HPP

#include <FreeRTOS.h>
#include <TestAppRtosInterface/Task.hpp>
#include <task.h>
#include <array>
#include <cstring>
#include <functional>

/// Namespace for Rtos Wrapper
namespace testapprtos {
/**
 * @brief
 * Inherited from @see class RtosTask it uses default properties of
 * an os task to setup a privileged level executing task.
 */
class TaskSetup : public Task {
public:
  /**
   * @brief: Constructor calling into parent with fixed privileged level.
   * @param[in] taskConfiguration @see rtosDef::rtosTaskConfiguration
   */
  explicit TaskSetup(const TaskConfiguration& taskConfiguration) : Task(taskConfiguration){};

protected:
private:
};

extern "C" void vApplicationGetIdleTaskMemory(
    StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer,
    uint32_t* pulIdleTaskStackSize);

// The external RTos Hook called when idle Task is reached it might be used to redirect into C++ space
extern "C" void vApplicationIdleHook();

}  // namespace testapprtos

#endif  // TESTAPPRTOS_HPP
