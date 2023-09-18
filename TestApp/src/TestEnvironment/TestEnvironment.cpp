/**
 * @file src/TestEnvironment/TestEnvironment.cpp
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

#include <TestEnvironment/TestEnvironment.hpp>
#include <TestSuite/TestSuiteInterface.hpp>
#include <TestCase/TestCaseInterface.hpp>

/**
 * @brief: testenvironment namespace.
 */
namespace testenvironment {
static message::TestExecutionMessage message{};
static testapp::ErrCode errCode = testapp::ErrCode::ERRCODE_UNDEFINED;
static size_t bytesReceived = 0;

testapp::ErrCode TestEnvironment::processMessage(const message::TestExecutionMessage& message) {
  testapp::ErrCode errCodeProcessMessage = testapp::ErrCode::ERRCODE_MSG_INCORRECT_RESPONSE;
  switch (message.subType) {
    case message::MessageSubType::TESTSUITE_SETUP:
      errCodeProcessMessage = static_cast<testsuite::TestSuiteInterface*>(message.testSuiteOrCase)->doSuiteSetup();
      break;
    case message::MessageSubType::TESTCASE_SETUP:
      errCodeProcessMessage = static_cast<testcase::TestCaseInterface*>(message.testSuiteOrCase)->doSetup();
      break;
    case message::MessageSubType::TESTCASE_RUN:
      errCodeProcessMessage = static_cast<testcase::TestCaseInterface*>(message.testSuiteOrCase)->runTest();
      break;
    case message::MessageSubType::TESTSUITE_TEARDOWN:
      errCodeProcessMessage = static_cast<testsuite::TestSuiteInterface*>(message.testSuiteOrCase)->doSuiteTearDown();
      break;
    case message::MessageSubType::TESTCASE_TEARDOWN:
      errCodeProcessMessage = static_cast<testcase::TestCaseInterface*>(message.testSuiteOrCase)->doTearDown();
      break;
    default:
      return testapp::ErrCode::ERRCODE_MSG_INCORRECT_RESPONSE;
      break;
  }
  return errCodeProcessMessage;
}

void TestEnvironment::runOneCycle() {
  // Check that the MessageBufferReferences was set.
  if (_testRunnerToTestEnvMessageBufferReference != nullptr && _testEnvironmentToTestRunnerMessageBufferReference != nullptr) {
    // Wait until there is a newly received message from TestRunner.
    bytesReceived = _testRunnerToTestEnvMessageBufferReference->receive(static_cast<void*>(&message), sizeof(message), portMAX_DELAY);
    if (bytesReceived > 0){
      errCode = processMessage(message);
    }

    // when the process is done, TestEnvironment shall send the final report to TestRunner
    if (errCode != testapp::ErrCode::ERRCODE_UNDEFINED) {
      message::MessageStructure reportMessage{};
      reportMessage.messageType = message::MessageType::Report;
      reportMessage.errorCode = errCode;
      reportMessage.subType = (errCode == testapp::ErrCode::SUCCESS) ? message::MessageSubType::TEST_PASS : message::MessageSubType::TEST_FAIL;

      _testEnvironmentToTestRunnerMessageBufferReference->send(static_cast<void*>(&reportMessage), sizeof(reportMessage), portMAX_DELAY);
    }
  }
}

void TestEnvironment::registerTestRunnerToTestEnvMessageBufferReference(osextension::MessageBufferReference& messageBufferReference) {
  _testRunnerToTestEnvMessageBufferReference = &messageBufferReference;
}

void TestEnvironment::registerTestEnvToTestRunnerMessageBufferReference(osextension::MessageBufferReference& messageBufferReference) {
  _testEnvironmentToTestRunnerMessageBufferReference = &messageBufferReference;
}

}  // namespace testenvironment
