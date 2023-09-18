/**
 * @file src/TestRunner/Task.cpp
 * @brief TestRunner Task
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
#include <TestRunner/Task.hpp>
#include <TestRegistry/TestRegistry.hpp>
#include <TestEnvironment/TaskInterface.hpp>

namespace testrunner
{  
  /// Create the task without using any dynamic memory allocation
  constexpr uint32_t taskStackSize = 2048u;
  StackType_t xStack[taskStackSize];

  /// Task descriptor
  const testapprtos::TaskConfiguration taskConfiguration = {
      taskStackSize,
      xStack,
      "testrunner",
      4u};

  /// Creation of global task object
  Task task{taskConfiguration};
  TaskInterface &taskInterface = task;
  
  /// Reference to TestRegistry singleton
  static TestRegistry &testRegistry = TestRegistry::getInstance();

  Task::Task(const testapprtos::TaskConfiguration &taskConfiguration) : TaskSetup(taskConfiguration)
  {
    // set testRunner MessageBuffer references
    _testRunner.setTestRunnerToTPCMessageBufferReference(_testRunnerToTPCMessageBuffer);
    _testRunner.setTPCToTestRunnerMessageBufferReference(_tpcToTestRunnerMessageBuffer);
    _testRunner.setTestRunnerToTestEnvironmentMessageBufferReference(_testRunnerToTestEnvironmentMessageBuffer);
    _testRunner.setTestEnvironmentToTestRunnerMessageBufferReference(_testEnvironmentToTestRunnerMessageBuffer);
    
    // Create static QueueSet
    _testRunner.setTestRunnerQueueSetHandle(osabstraction::queueCreateSetStatic(numberOfEventsInQueueSet, _queueSetStorage.data(), &_staticQueue));

    // Add all semaphores to the QueueSet
    if (xQueueAddToSet(static_cast<QueueSetMemberHandle_t>(_testEnvironmentToTestRunnerMessageBuffer.getHandleForQueueSet()), _testRunner.getTestRunnerQueueSetHandle()) != pdTRUE)
    {
      assert(false);
    }
    if (xQueueAddToSet(static_cast<QueueSetMemberHandle_t>(_tpcToTestRunnerMessageBuffer.getHandleForQueueSet()), _testRunner.getTestRunnerQueueSetHandle()) != pdTRUE)
    {
      assert(false);
    }
  }

  // Get Reference of the incoming message buffer from TPCoordinator
  osextension::MessageBufferReference Task::getTPCToTestRunnerMessageBufferReference() const
  {
    return _tpcToTestRunnerMessageBuffer.getReference();
  }

  osextension::MessageBufferReference Task::getTestRunnerToTPCMessageBufferReference() const
  {
    return _testRunnerToTPCMessageBuffer.getReference();
  }

  /**
   * @brief run function
   */
  void Task::run()
  {
    // Register the message buffer reference to _testRunnerToTestEnvironmentMessageBuffer
    testenvironment::taskInterface.registerTestRunnerToTestEnvMessageBufferReference(_testRunnerToTestEnvironmentMessageBuffer);

    // Register the message buffer reference to _testEnvironmentToTestRunnerMessageBuffer
    testenvironment::taskInterface.registerTestEnvToTestRunnerMessageBufferReference(_testEnvironmentToTestRunnerMessageBuffer);

    QueueSetMemberHandle_t queueSetMemberHandle = nullptr;

    while (true)
    {
      _testRunner.oneCycle(queueSetMemberHandle);
    }
  } // Task::Run
} // namespace testrunner
