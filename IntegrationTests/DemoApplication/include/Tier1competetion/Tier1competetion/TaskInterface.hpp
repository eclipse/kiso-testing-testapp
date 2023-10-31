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

#ifndef TIER1COMPETETION_TASKINTERFACE_HPP
#define TIER1COMPETETION_TASKINTERFACE_HPP

#include <OsExtension/MessageBuffer.hpp>
#include <Tier1competetion/Tier1competetion.hpp>

#include <utility>

/**
 * @brief: tier1competetion namespace.
 */
namespace tier1competetion {

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
   * @brief Register MessageBufferReference from Dashboar to TestEnvironment.
   * @param[in] messageBufferReference Reference to the message buffer.
   */
  virtual void registerDashboardToTier1competetionMessageBufferReference(osextension::MessageBufferReference& messageBufferReference) = 0;

  /**
   * @brief Register MessageBufferReference from TestEnvironment to Dashboar.
   * @param[in] messageBufferReference Reference to the message buffer.
   */  
  virtual void registerTier1competetionToDashboardMessageBufferReference(osextension::MessageBufferReference& messageBufferReference) = 0;
  
  /// Initialize
  virtual void initialize() = 0;

  Tier1competetion tier1competetion;             /**< Tier1competetion instance. */

protected:
   /// Default constructor
  TaskInterface() = default;

  /// Destructor
  virtual ~TaskInterface() = default;
};

/// Global interface to task
extern TaskInterface& taskInterface;

}  // namespace tier1competetion.

#endif  // TIER1COMPETETION_TASKINTERFACE_HPP
