/**
 * @file include/TestEnvironment/TestEnvironment/TaskInterface.hpp
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

#ifndef TESTENVIRONMENT_TASKINTERFACE_HPP
#define TESTENVIRONMENT_TASKINTERFACE_HPP

#include <OsExtension/MessageBuffer.hpp>
#include <utility>

/**
 * @brief: testenvironment namespace.
 */
namespace testenvironment {

/**
 * @brief: Interface for the Task class, which executes the TestEnvironment functionality.
 */
class TaskInterface {
public:
  TaskInterface(const TaskInterface&) = delete;                  /**< Deleted copy constructor. */
  TaskInterface(const TaskInterface&&) = delete;                 /**< Deleted move constructor. */
  const TaskInterface& operator=(const TaskInterface&) = delete; /**< Deleted copy assignment operator. */
  const TaskInterface& operator=(TaskInterface&&) = delete;      /**< Deleted move assignment operator. */

  /**
   * @brief Register MessageBufferReference from TestRunner to TestEnvironment.
   * @param[in] messageBufferReference Reference to the message buffer.
   */
  virtual void registerTestRunnerToTestEnvMessageBufferReference(osextension::MessageBufferReference& messageBufferReference) = 0;

  /**
   * @brief Register MessageBufferReference from TestEnvironment to TestRunner.
   * @param[in] messageBufferReference Reference to the message buffer.
   */
  virtual void registerTestEnvToTestRunnerMessageBufferReference(osextension::MessageBufferReference& messageBufferReference) = 0;

  /**
   * @brief Initialize.
   */
  virtual void initialize() = 0;

protected:
  /// Default constructor.
  TaskInterface() = default;

  /// Destructor.
  virtual ~TaskInterface() = default;
};

/// Global interface to task.
extern TaskInterface& taskInterface;

}  // namespace testenvironment.

#endif  // TESTENVIRONMENT_TASKINTERFACE_HPP
