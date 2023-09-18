/**
 * @file include/Dashboard/Dashboard/TaskInterface.hpp
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

#ifndef DASHBOARD_TASKINTERFACE_HPP
#define DASHBOARD_TASKINTERFACE_HPP

#include <OsExtension/MessageBuffer.hpp>
#include <utility>

/// @brief dashboard task namespace
namespace dashboard {

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
   * @brief Get reference to _tier2competetionToDashboardMessageBuffer
   * @return MessageBufferReference to _tier2competetionToDashboardMessageBuffer
   */
  virtual osextension::MessageBufferReference getTier2competetionToDashboardMessageBufferReference() const = 0;

  /**
   * @brief Get reference to _dashboardToTier2competetionMessageBuffer
   * @return MessageBufferReference to _dashboardToTier2competetionMessageBuffer
   */
  virtual osextension::MessageBufferReference getDashboardToTier2competetionMessageBufferReference() const = 0;

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

}  // namespace dashboard

#endif  // DASHBOARD_TASKINTERFACE_HPP
