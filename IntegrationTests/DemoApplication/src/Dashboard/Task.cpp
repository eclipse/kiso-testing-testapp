/**
 * @file src/Dashboard/Task.cpp
 * @brief Dashboard Task
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 */
#include <Dashboard/Task.hpp>
#include <Tier1competetion/TaskInterface.hpp>

namespace dashboard
{
  /// Create the task without using any dynamic memory allocation
  constexpr uint32_t taskStackSize = 2048u;
  StackType_t xStack[ taskStackSize ];

  /// Task descriptor
  const testapprtos::TaskConfiguration taskConfiguration = {
      taskStackSize,
      xStack,
      "dashboard",
      4u
  };

  /// Creation of global task object
  Task task{taskConfiguration};
  TaskInterface &taskInterface = task;

  Task::Task(const testapprtos::TaskConfiguration &taskConfiguration) : TaskSetup(taskConfiguration)
  {
    // set dashboard MessageBuffer references
    _dashboard.setDashboardToTier2competetionMessageBufferReference(_dashboardToTier2competetionMessageBuffer);
    _dashboard.setTier2competetionToDashboardMessageBufferReference(_tier2competetionToDashboardMessageBuffer);
    _dashboard.setDashboardToTier1competetionMessageBufferReference(_dashboardToTier1competetionMessageBuffer);
    _dashboard.setTier1competetionToDashboardMessageBufferReference(_tier1competetionToDashboardMessageBuffer);
    
    // Create static QueueSet
    _dashboard.setDashboardQueueSetHandle(osabstraction::queueCreateSetStatic(numberOfEventsInQueueSet, _queueSetStorage.data(), &_staticQueue));

    // Add all semaphores to the QueueSet
    if (xQueueAddToSet(static_cast<QueueSetMemberHandle_t>(_tier1competetionToDashboardMessageBuffer.getHandleForQueueSet()), _dashboard.getDashboardQueueSetHandle()) != pdTRUE)
    {
      assert(false);
    }
    if (xQueueAddToSet(static_cast<QueueSetMemberHandle_t>(_tier2competetionToDashboardMessageBuffer.getHandleForQueueSet()), _dashboard.getDashboardQueueSetHandle()) != pdTRUE)
    {
      assert(false);
    }
  }

  // Get Reference of the incoming message buffer from Tier2competetionoordinator
  osextension::MessageBufferReference Task::getTier2competetionToDashboardMessageBufferReference() const
  {
    return _tier2competetionToDashboardMessageBuffer.getReference();
  }

  osextension::MessageBufferReference Task::getDashboardToTier2competetionMessageBufferReference() const
  {
    return _dashboardToTier2competetionMessageBuffer.getReference();
  }

  /**
   * @brief run function
   */
  void Task::run()
  {
    // Register the message buffer reference to _dashboardToTier1competetionMessageBuffer
    tier1competetion::taskInterface.registerDashboardToTier1competetionMessageBufferReference(_dashboardToTier1competetionMessageBuffer);

    // Register the message buffer reference to _tier1competetionToDashboardMessageBuffer
    tier1competetion::taskInterface.registerTier1competetionToDashboardMessageBufferReference(_tier1competetionToDashboardMessageBuffer);

    QueueSetMemberHandle_t queueSetMemberHandle = nullptr;

    while (true)
    {
      _dashboard.oneCycle(queueSetMemberHandle);
    }
  } // Task::Run
} // namespace testrunner
