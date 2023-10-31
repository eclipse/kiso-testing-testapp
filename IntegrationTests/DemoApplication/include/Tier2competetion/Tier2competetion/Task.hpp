/**
 * @file include/TIER2COMPETETION/TIER2COMPETETION/Task.hpp
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

#ifndef TIER2COMPETETION_TASK_HPP
#define TIER2COMPETETION_TASK_HPP
#include "Dashboard/TaskInterface.hpp"
#include "Tier2competetion/TaskInterface.hpp"
#include "Tier2competetion/Tier2competetion.hpp"
// #include <TestAppRtosInterface/ConfigureTasks.hpp>
#include <TestAppRtos/Rtos.hpp>

namespace tier2competetion {
/**
 * @brief Tier2competetion class
 */
class Task : public testapprtos::TaskSetup, public TaskInterface {
public:
  /**
   * @brief Constructor
   * @param[in] taskConfiguration Task configuration
   */
  explicit Task(const testapprtos::TaskConfiguration &taskConfiguration);

  /**
   * @brief run function
   */
  void run() override;

  /// @copydoc Task::initializeTask()
  void initialize() override { initializeTask(); }

private:
  Task() = delete;              /**< Remove empty (default) constructor */
  Task(const Task &) = delete;  /**< Remove copy Constructor */
  Task(const Task &&) = delete; /**< Remove assign Constructor */
  const Task &
  operator=(const Task &) = delete; /**< Remove assign Operation @return NULL */
  const Task &
  operator=(Task &&) = delete; /**< Remove move Operation @return NULL */

  /// Size of message buffer in bytes for incoming message bus messages
  static constexpr size_t IncomingMessageBufferSize = 265u;

  /// Maximum number of messages in message buffer for incoming message bus
  /// messages
  static constexpr size_t IncomingMessageBufferMaximumNumberOfMessages = 1u;

  /// Message Buffer reference for incoming messages from Dashboard
  osextension::MessageBufferReference _dashboardToTier2competetionMessageBuffer;

  /// Message Buffer reference for Outgoing messages to Dashboard
  osextension::MessageBufferReference _tier2competetionToDashboardMessageBuffer;

  /// Semaphore
  osextension::Semaphore _TCCInterfacerReferencePairAdded{1, 0};

  /// Tier2competetion instance
  Tier2competetion _tier2competetion{};

  /// Task delay time
  TickType_t _blockingTime{0};
};

} // namespace tier2competetion

#endif
