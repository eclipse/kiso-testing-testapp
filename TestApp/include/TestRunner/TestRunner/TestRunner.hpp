/**
 * @file include/TestRunner/TestRunner/TestRunner.hpp
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 */

#ifndef TESTRUNNER_TESTRUNNER_HPP
#define TESTRUNNER_TESTRUNNER_HPP

#include <MessageHandler/MessageHandler.hpp>
#include <TestRegistry/TestRegistry.hpp>
#include <OsExtension/MessageBuffer.hpp>

namespace testrunner {

class TestRunner {
public:
  /// Constructor.
  TestRunner() = default;

  /// Default destructor
  ~TestRunner() = default;

  /// Copy constructor.
  TestRunner(TestRunner const&) = delete;

  /// Move constructor.
  TestRunner(TestRunner&&) = delete;

  /// @brief Copy assignment operator.
  /// @return result of copy assignment
  TestRunner& operator=(TestRunner const&) = delete;

  /// @brief Move assignment operator.
  /// @return result of move assignment
  TestRunner& operator=(TestRunner&&) = delete;
  
  /**
   * @brief This is the callback method which handles the data received from ITF to process the command to execute TestSuite and Testcase
   *
   * @return testapp::ErrCode
   */
  testapp::ErrCode processTestCmd(const message::MessageStructure &command, message::TestExecutionMessage& dataSentToTestEnv);
  
  /**
   * @brief Method to send TestReport to ITF using TPC data sharing mechanism
   *
   * @return testapp::ErrCode
   */
  void sendReport(message::MessageStructure &report, const message::MessageStructure &dataReceivedFromTPC);

  /**
   * @brief one cycle Method to be run by TestRuner task
   */
  void oneCycle(QueueSetMemberHandle_t& queueSetMemberHandle);

  /**
   * @brief set _testRunnerToTPCMessageBuffer Reference
   */
  void setTestRunnerToTPCMessageBufferReference(osextension::MessageBufferReference& mbf);

  /**
   * @brief set _tpcToTestRunnerMessageBuffer Reference
   */
  void setTPCToTestRunnerMessageBufferReference(osextension::MessageBufferReference& mbf);

  /**
   * @brief set _testRunnerToTestEnvironmentMessageBuffer Reference
   */
  void setTestRunnerToTestEnvironmentMessageBufferReference(osextension::MessageBufferReference& mbf);

  /**
   * @brief set _testEnvironmentToTestRunnerMessageBuffer Reference
   */
  void setTestEnvironmentToTestRunnerMessageBufferReference(osextension::MessageBufferReference& mbf);

  /**
   * @brief set testRunner queue set handle
   */
  void setTestRunnerQueueSetHandle(QueueSetHandle_t queueSetHandle);

  /**
   * @brief get testRunner queue set handle
   */
  QueueSetHandle_t getTestRunnerQueueSetHandle() const;

private:
  /// Message Buffer reference for incoming messages from TestEnv
  osextension::MessageBufferReference* _testEnvironmentToTestRunnerMessageBufferReference = nullptr;

  /// Message Buffer reference for incoming messages from TPC
  osextension::MessageBufferReference* _tpcToTestRunnerMessageBufferReference = nullptr;

  /// Outgoing Message Buffer reference to the TestEnv
  osextension::MessageBufferReference* _testRunnerToTestEnvironmentMessageBufferReference = nullptr;

  /// Outgoing Message Buffer reference to the TPC
  osextension::MessageBufferReference* _testRunnerToTPCMessageBufferReference = nullptr;

  /// FreeRTOS queue set handle
  QueueSetHandle_t _queueSetHandle = nullptr;
};

}  // namespace testrunner

#endif // TESTRUNNER_TESTRUNNER_HPP
