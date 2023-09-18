/**
 * @file include/TPC/TPC/Task.hpp
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

#ifndef TPC_TASK_HPP
#define TPC_TASK_HPP
#include "TestRunner/TaskInterface.hpp"
#include "TPC/TaskInterface.hpp"
#include "TPC/TPC.hpp"
#include <TestAppTypes/TestAppTypes.hpp>
#include <MessageHandler/MessageHandler.hpp>
#include <TestAppRtos/Rtos.hpp>

namespace tpcprotocol{
/**
 * @brief TPCProtocol class
 */
class Task : public testapprtos::TaskSetup, public TaskInterface {
public:
  /**
   * @brief Constructor
   * @param[in] taskConfiguration Task configuration
   */
  explicit Task(const testapprtos::TaskConfiguration& taskConfiguration);

  /**
   * @brief run function
   */
  void run() override;

  /// @copydoc Task::initializeTask()
  void initialize() override {
    initializeTask();
  }

private:
  Task() = delete;                             /**< Remove empty (default) constructor */
  Task(const Task&) = delete;                  /**< Remove copy Constructor */
  Task(const Task&&) = delete;                 /**< Remove assign Constructor */
  const Task& operator=(const Task&) = delete; /**< Remove assign Operation @return NULL */
  const Task& operator=(Task&&) = delete;      /**< Remove move Operation @return NULL */

  /// Size of message buffer in bytes for incoming message bus messages
  static constexpr size_t IncomingMessageBufferSize = 265u;

  /// Maximum number of messages in message buffer for incoming message bus messages
  static constexpr size_t IncomingMessageBufferMaximumNumberOfMessages = 1u;

  /// Message Buffer reference for incoming messages from TestRunner
  osextension::MessageBufferReference _TestRunnerToTPCMessageBuffer;

  /// Message Buffer reference for Outgoing messages to TestRunner
  osextension::MessageBufferReference _TPCToTestRunnerMessageBuffer;

    /// Semaphore
  osextension::Semaphore _TCCInterfacerReferencePairAdded{1,0};

  ///TPC instance
  TPC _tpc{};

  /// Task delay time
  TickType_t _blockingTime{0};
};

} //namespace TPCProtocol

#endif
