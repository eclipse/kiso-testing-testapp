/**
 * @file include/TPC/TPC/TaskInterface.hpp
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

#ifndef TPC_TPCINTERFACE_HPP
#define TPC_TPCINTERFACE_HPP

#include <OsExtension/MessageBuffer.hpp>
#include <TestAppTypes/TestAppTypes.hpp>
#include <utility>

/// @brief testrunner task namespace
namespace tpcprotocol {
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

}  // namespace tpcprotocol

#endif  // TPC_TPCINTERFACE_HPP
