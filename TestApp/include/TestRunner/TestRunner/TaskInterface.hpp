/**
 * @file include/TestRunner/TestRunner/TaskInterface.hpp
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

#ifndef TESTRUNNER_TASKINTERFACE_HPP
#define TESTRUNNER_TASKINTERFACE_HPP

#include <OsExtension/MessageBuffer.hpp>
#include <utility>

/// @brief testrunner task namespace
namespace testrunner {

/**
 * @brief: Task Interface
 */
class TaskInterface {
public:
  /// Deleted copy constructor
  TaskInterface(const TaskInterface&) = delete;

  /// Deleted move constructor
  TaskInterface(const TaskInterface&&) = delete;

  /// Deleted copy assignment operator
  const TaskInterface& operator=(const TaskInterface&) = delete;

  /// Deleted move assignment operator
  const TaskInterface& operator=(TaskInterface&&) = delete;

  /**
   * @brief Get reference to _tpcToTestRunnerMessageBuffer
   * @return MessageBufferReference to _tpcToTestRunnerMessageBuffer
   */
  virtual osextension::MessageBufferReference getTPCToTestRunnerMessageBufferReference() const = 0;

  /**
   * @brief Get reference to _testRunnerToTPCMessageBuffer
   * @return MessageBufferReference to _testRunnerToTPCMessageBuffer
   */
  virtual osextension::MessageBufferReference getTestRunnerToTPCMessageBufferReference() const = 0;

  /// Initialize
  virtual void initialize() = 0;

protected:
  /// Default constructor
  TaskInterface() = default;

  /// Destructor
  virtual ~TaskInterface() = default;
};

/// Global interface to task
extern TaskInterface& taskInterface;

}  // namespace testrunner

#endif  // TESTRUNNER_TASKINTERFACE_HPP
