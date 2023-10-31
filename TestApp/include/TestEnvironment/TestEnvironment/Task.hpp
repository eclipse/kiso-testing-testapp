/**
 * @file include/TestEnvironment/TestEnvironment/Task.hpp
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

#ifndef TESTENVIRONMENT_TASK_HPP
#define TESTENVIRONMENT_TASK_HPP

#include <TestEnvironment/TaskInterface.hpp>
#include <TestEnvironment/TestEnvironment.hpp>
#include <TestAppRtos/Rtos.hpp>
#include <etl/map.h>
#include <etl/vector.h>
#include <utility>

/**
 * @brief: testenvironment namespace.
 */
namespace testenvironment {

/**
 * @brief Task class, which executes the TestEnvironment functionality.
 */
class Task : public testapprtos::TaskSetup, public TaskInterface {
public:
  /**
   * @brief Constructor.
   * @param[in] taskConfiguration Task configuration.
   */
  explicit Task(const testapprtos::TaskConfiguration& taskConfiguration);

  /**
   * @brief Register MessageBufferReference from TestRunner to TestEnvironment.
   * @param[in] messageBufferReference Reference to the message buffer.
   */
  void registerTestRunnerToTestEnvMessageBufferReference(osextension::MessageBufferReference& messageBufferReference) override;

  /**
   * @brief Register MessageBufferReference from TestEnvironment to TestRunner.
   * @param[in] messageBufferReference Reference to the message buffer.
   */
  void registerTestEnvToTestRunnerMessageBufferReference(osextension::MessageBufferReference& messageBufferReference) override;

  /**
   * @brief Run function.
   */
  void run() override;

  /**
   * @brief Initialize.
   */
  void initialize() override;

private:
  Task() = delete;                             /**< Remove empty (default) constructor. */
  Task(const Task&) = delete;                  /**< Remove copy Constructor. */
  Task(const Task&&) = delete;                 /**< Remove assign Constructor. */
  const Task& operator=(const Task&) = delete; /**< Remove assign Operation @return NULL. */
  const Task& operator=(Task&&) = delete;      /**< Remove move Operation @return NULL. */
  TestEnvironment testEnvironment;             /**< TestEnvironment instance. */
  /**
   * @brief Semaphore to signal, that the message buffer from TestRunnter to TestEnvironment was registered.
   */
  osextension::Semaphore _testRunnerToTestEnvMessageBufferAdded{1, 0};
  /**
   * @brief Semaphore to signal, that the message buffer from TestEnvironment to TestRunnter was registered.
   */
  osextension::Semaphore _testEnvToTestRunnerMessageBufferAdded{1, 0};
};

}  // namespace testenvironment

#endif  // TESTENVIRONMENT_TASK_HPP
