/**
 * @file src/TestEnvironment/Task.cpp
 * @brief TestEnvironment Task
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

#include <TestEnvironment/Task.hpp>
#include <TestEnvironment/Logging.hpp>

/**
 * @brief: testenvironment namespace.
 */
namespace testenvironment
{
  /// Create the task without using any dynamic memory allocation
  constexpr uint32_t taskStackSize = 2048u;
  StackType_t xStack[taskStackSize];

  /// Task descriptor
  const testapprtos::TaskConfiguration taskConfiguration = {
      taskStackSize,
      xStack,
      "testenvironment",
      4u};

  /// Creation of global task object.
  Task task{taskConfiguration};
  TaskInterface &taskInterface = task;

  Task::Task(const testapprtos::TaskConfiguration &taskConfiguration) : TaskSetup(taskConfiguration) {}

  void Task::registerTestRunnerToTestEnvMessageBufferReference(osextension::MessageBufferReference &messageBufferReference)
  {
    testEnvironment.registerTestRunnerToTestEnvMessageBufferReference(messageBufferReference);
    _testRunnerToTestEnvMessageBufferAdded.signal();
  }

  void Task::registerTestEnvToTestRunnerMessageBufferReference(osextension::MessageBufferReference &messageBufferReference)
  {
    testEnvironment.registerTestEnvToTestRunnerMessageBufferReference(messageBufferReference);
    testenvironment::Logging::getInstance().setMessageBuffer(messageBufferReference);
    _testEnvToTestRunnerMessageBufferAdded.signal();
  }

  void Task::run()
  {
    // Wait until the input and output message buffer were registered.
    _testRunnerToTestEnvMessageBufferAdded.wait(portMAX_DELAY);
    _testEnvToTestRunnerMessageBufferAdded.wait(portMAX_DELAY);
    while (true)
    {
      testEnvironment.runOneCycle();
    }
  }

  void Task::initialize() { initializeTask(); }

} // namespace testenvironment
