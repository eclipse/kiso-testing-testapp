/**
 * @file src/Tier1competetion/Task.cpp
 * @brief Tier1competetion Task
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

#include <Tier1competetion/Task.hpp>

/**
 * @brief: Tier1competetion namespace.
 */
namespace tier1competetion
{
  /// Create the task without using any dynamic memory allocation
  constexpr uint32_t taskStackSize = 2048u;
  StackType_t xStack[ taskStackSize ];

  /// Task descriptor
  const testapprtos::TaskConfiguration taskConfiguration = {
      taskStackSize,
      xStack,
      "tier1competetion",
      4u
  };

  /// Creation of global task object
  Task task{taskConfiguration};
  TaskInterface &taskInterface = task;

  Task::Task(const testapprtos::TaskConfiguration &taskConfiguration) : TaskSetup(taskConfiguration) {}

  void Task::registerDashboardToTier1competetionMessageBufferReference(osextension::MessageBufferReference &messageBufferReference)
  {
    tier1competetion.registerDashboardToTier1competetionMessageBufferReference(messageBufferReference);
    _dashboardToTier1competetionMessageBufferAdded.signal();
  }

  void Task::registerTier1competetionToDashboardMessageBufferReference(osextension::MessageBufferReference &messageBufferReference)
  {
    tier1competetion.registerTier1competetionToDashboardMessageBufferReference(messageBufferReference);
    _tier1competetionToDashboardMessageBufferAdded.signal();
  }

  void Task::run()
  {
    // Wait until the input and output message buffer were registered.
    _dashboardToTier1competetionMessageBufferAdded.wait(portMAX_DELAY);
    _tier1competetionToDashboardMessageBufferAdded.wait(portMAX_DELAY);
    while (true)
    {
      tier1competetion.runOneCycle();
    }
  }

} // namespace tier1competetion
