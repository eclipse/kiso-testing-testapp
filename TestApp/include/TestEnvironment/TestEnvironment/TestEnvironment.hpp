/**
 * @file include/TestEnvironment/TestEnvironment/TestEnvironment.hpp
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

#ifndef TESTENVIRONMENT_TESTENVIRONMENT_HPP
#define TESTENVIRONMENT_TESTENVIRONMENT_HPP

#include <OsExtension/MessageBuffer.hpp>
#include <TestAppTypes/TestAppTypes.hpp>
#include <MessageHandler/MessageHandler.hpp>

/**
 * @brief: testenvironment namespace.
 */
namespace testenvironment {

/**
 * @brief: TestEnvironment class.
 */
class TestEnvironment {
public:
  /**
   * @brief Run one cycle.
   */
  void runOneCycle();

  /**
   * @brief Register MessageBufferReference from TestRunner to TestEnvironment.
   * @param[in] messageBufferReference Reference to the message buffer.
   */
  void registerTestRunnerToTestEnvMessageBufferReference(osextension::MessageBufferReference& messageBufferReference);

  /**
   * @brief Register MessageBufferReference from TestEnvironment To TestRunner.
   * @param[in] messageBufferReference Reference to the message buffer.
   */
  void registerTestEnvToTestRunnerMessageBufferReference(osextension::MessageBufferReference& messageBufferReference);

private:
  /**
   * @brief Processes a message.
   * @param[in] message Message to process.
   */
  static testapp::ErrCode processMessage(const message::TestExecutionMessage& message);

  /**
   * @brief MessageBufferReference from TestRunner to TestEnvironment.
   */
  osextension::MessageBufferReference* _testRunnerToTestEnvMessageBufferReference = nullptr;

  /**
   * @brief MessageBufferReference from TestEnvironment to TestRunner.
   */
  osextension::MessageBufferReference* _testEnvironmentToTestRunnerMessageBufferReference = nullptr;
};

}  // namespace testenvironment

#endif  // TESTENVIRONMENT_TESTENVIRONMENT_HPP
