/**
 * @file include/TestRunner/TestRunner/Task.hpp
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

#ifndef TESTRUNNER_TASK_HPP
#define TESTRUNNER_TASK_HPP

#include <TestRunner/TaskInterface.hpp>
#include <TestRunner/TestRunner.hpp>
#include <TestAppTypes/TestAppTypes.hpp>
#include <MessageHandler/MessageHandler.hpp>
#include <TestAppRtos/Rtos.hpp>

#include <etl/map.h>
#include <etl/vector.h>

#include <utility>

/// @brief testrunner task namespace
namespace testrunner {

/**
 * @brief testrunner class
 */
class Task : public testapprtos::TaskSetup, public TaskInterface {
public:
  /**
   * @brief Constructor
   * @param[in] taskConfiguration Task configuration
   */
  explicit Task(const testapprtos::TaskConfiguration& taskConfiguration);

  /**
   * @brief Run function
   */
  void run() override;

  /// @copydoc Task::initializeTask()
  void initialize() override {
    initializeTask();
  }

  /// @copydoc TaskInterface::getTestRunnerToTPCMessageBufferReference
  osextension::MessageBufferReference getTestRunnerToTPCMessageBufferReference() const override;

  /// @copydoc TaskInterface::getTPCToTestRunnerMessageBufferReference
  osextension::MessageBufferReference getTPCToTestRunnerMessageBufferReference() const override;

private:
  Task() = delete;                             /**< Remove empty (default) constructor */
  Task(const Task&) = delete;                  /**< Remove copy Constructor */
  Task(const Task&&) = delete;                 /**< Remove assign Constructor */
  const Task& operator=(const Task&) = delete; /**< Remove assign Operation @return NULL */
  const Task& operator=(Task&&) = delete;      /**< Remove move Operation @return NULL */

  /// Size of message buffer in bytes for incoming message bus messages
  static constexpr size_t IncomingMessageBufferSize = sizeof(message::MessageStructure) + 5u; //265 bytes

  /// Maximum number of messages in message buffer for incoming message bus messages
  static constexpr size_t IncomingMessageBufferMaximumNumberOfMessages = 10u;

  /// Size of interrupt queue (= number of buttons)
  static constexpr size_t sizeOfInterruptQueue = 1u;

  /**
   * @brief Length of QueueSet inside TaskWakeUp object (number of events which has the QueueSet to handle at all)
   * @note Must be set to the sum of the lengths of the queues added to the set, where binary semaphores
   *       and mutexes have a length of 1, and counting semaphores have a length set by their maximum count value
   */
  static constexpr auto numberOfEventsInQueueSet = IncomingMessageBufferMaximumNumberOfMessages + sizeOfInterruptQueue + testapprtos::maximumNumberOfEvents;

  /// Message buffer for incoming messages from TestEnv
  osextension::MessageBuffer<IncomingMessageBufferSize, IncomingMessageBufferMaximumNumberOfMessages> _testEnvironmentToTestRunnerMessageBuffer{};

  /// Message buffer for incoming messages from TPC
  osextension::MessageBuffer<IncomingMessageBufferSize, IncomingMessageBufferMaximumNumberOfMessages> _tpcToTestRunnerMessageBuffer{};

  /// Outgoing message buffer to the TestEnv
  osextension::MessageBuffer<IncomingMessageBufferSize, IncomingMessageBufferMaximumNumberOfMessages> _testRunnerToTestEnvironmentMessageBuffer{};

  /// Outgoing message buffer to the TPC
  osextension::MessageBuffer<IncomingMessageBufferSize, IncomingMessageBufferMaximumNumberOfMessages> _testRunnerToTPCMessageBuffer{};

  /// Handle type of FreeRTOS
  using HandleType = void*;

  /// FreeRTOS queue set struct
  StaticQueue_t _staticQueue{};

  /// Queue set storage area
  std::array<uint8_t, numberOfEventsInQueueSet * sizeof(HandleType)> _queueSetStorage = {0u};

  /// TestRunner Instance
  TestRunner _testRunner{};
};

}  // namespace testrunner

#endif  // TESTRUNNER_TASK_HPP
