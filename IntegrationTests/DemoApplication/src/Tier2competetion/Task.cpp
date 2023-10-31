/**
 * @file src/Tier2competetion/Task.cpp
 * @brief Tier2competetion Task
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

#include <Tier2competetion/Task.hpp>
#include <cstdint>
namespace tier2competetion
{
  /// Create the task without using any dynamic memory allocation
  constexpr uint32_t taskStackSize = 2048u;
  StackType_t xStack[ taskStackSize ];

  /// Task descriptor
  const testapprtos::TaskConfiguration taskConfiguration = {
      taskStackSize,
      xStack,
      "tier2competetion",
      4u
  };

  /// Creation of global task object
  Task task{taskConfiguration};
  TaskInterface &taskInterface = task;

  Task::Task(const testapprtos::TaskConfiguration &taskConfiguration) : TaskSetup(taskConfiguration)
  {
    // register callbacks
    _tier2competetion.setTier2competetionToDashboardMessageBufferReference(_tier2competetionToDashboardMessageBuffer);
    _tier2competetion.setDashboardTier2competetionMessageBufferReference(_dashboardToTier2competetionMessageBuffer);
  }

  /**
   * @brief run function
   */
  void Task::run()
  {
    // Insert the message buffer reference to TestCoordinator
    _dashboardToTier2competetionMessageBuffer = dashboard::taskInterface.getDashboardToTier2competetionMessageBufferReference();

    // Register the message buffer reference of the Tier2competetion to Dashboard task
    _tier2competetionToDashboardMessageBuffer = dashboard::taskInterface.getTier2competetionToDashboardMessageBufferReference();

    _tier2competetion.oneCycle();
  }
} // tier2competetion
