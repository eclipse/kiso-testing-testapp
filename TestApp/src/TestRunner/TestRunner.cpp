/**
 * @file src/TestRunner/TestRunner/TestRunner.cpp
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

#include <TestRunner/TestRunner.hpp>
#include <TestRegistry/TestRegistry.hpp>

namespace testrunner
{
  /// Reference to TestRegistry singleton
  static TestRegistry &testRegistry = TestRegistry::getInstance();

  /// set testRunner message Buffer references
  void TestRunner::setTestRunnerToTPCMessageBufferReference(osextension::MessageBufferReference& mbf) {
    _testRunnerToTPCMessageBufferReference = &mbf;
  }
  void TestRunner::setTPCToTestRunnerMessageBufferReference(osextension::MessageBufferReference& mbf) {
    _tpcToTestRunnerMessageBufferReference = &mbf;
  }
  void TestRunner::setTestRunnerToTestEnvironmentMessageBufferReference(osextension::MessageBufferReference& mbf) {
    _testRunnerToTestEnvironmentMessageBufferReference = &mbf;
  }
  void TestRunner::setTestEnvironmentToTestRunnerMessageBufferReference(osextension::MessageBufferReference& mbf) {
    _testEnvironmentToTestRunnerMessageBufferReference = &mbf;
  }

  /// testRunner queue set setter and getter
  void TestRunner::setTestRunnerQueueSetHandle(QueueSetHandle_t queueSetHandle) {
    _queueSetHandle = queueSetHandle;
  }
  QueueSetHandle_t TestRunner::getTestRunnerQueueSetHandle() const {
    return _queueSetHandle;
  }

  void TestRunner::oneCycle(QueueSetMemberHandle_t &queueSetMemberHandle) {
    // Wait until there is something to do
    queueSetMemberHandle = xQueueSelectFromSet(_queueSetHandle, pdMS_TO_TICKS(portMAX_DELAY));
    message::MessageStructure dataReceivedFromTPC;
    message::MessageStructure dataReceivedFromTestEnv;
    message::TestExecutionMessage dataSentToTestEnv;
    message::MessageStructure reportToTPC;
    testapp::ErrCode retCode;

    if (queueSetMemberHandle != nullptr) {
      if (queueSetMemberHandle == _tpcToTestRunnerMessageBufferReference->getHandleForQueueSet()) {
        size_t receivedBytesFromTPC = _tpcToTestRunnerMessageBufferReference->receive(static_cast<void*>(&dataReceivedFromTPC),
                                                                              sizeof(message::MessageStructure), portMAX_DELAY);
        if (receivedBytesFromTPC > 0) {
          switch (dataReceivedFromTPC.messageType) {
            case message::MessageType::Command:
              // ProcessCmd e.g. send the entire message
              retCode = processTestCmd(dataReceivedFromTPC, dataSentToTestEnv);
              // check for error and send a report message to TPC
              if (testapp::ErrCode::SUCCESS != retCode) {
                reportToTPC.errorCode = retCode;
                reportToTPC.messageType = message::MessageType::Report;
                reportToTPC.subType = message::MessageSubType::TEST_FAIL;
                sendReport(reportToTPC, dataReceivedFromTPC);
              }
              break;
            case message::MessageType::Acknowledgement:
            case message::MessageType::Report:
            default:
              // send error message to TPC
              reportToTPC.errorCode = testapp::ErrCode::ERRCODE_MSG_INCORRECT_RESPONSE;
              reportToTPC.messageType = message::MessageType::Report;
              reportToTPC.subType = message::MessageSubType::TEST_FAIL;
              sendReport(reportToTPC, dataReceivedFromTPC);
              break;
          }
        }
        else {
          // send error message to TPC
          reportToTPC.errorCode = testapp::ErrCode::ERRCODE_MSG_INCOMPLETE;
          reportToTPC.messageType = message::MessageType::Report;
          reportToTPC.subType = message::MessageSubType::TEST_FAIL;
          sendReport(reportToTPC, dataReceivedFromTPC);
        }
      }
      else if (queueSetMemberHandle == _testEnvironmentToTestRunnerMessageBufferReference->getHandleForQueueSet()) {
        // Receive Message from TestEnvironment
        size_t receivedBytesFromTestEnv = _testEnvironmentToTestRunnerMessageBufferReference->receive(static_cast<void*>(&dataReceivedFromTestEnv),
                                                                                              sizeof(dataReceivedFromTestEnv), portMAX_DELAY);
        if (receivedBytesFromTestEnv > 0u &&
          (dataReceivedFromTestEnv.messageType == message::MessageType::Report ||
          dataReceivedFromTestEnv.messageType == message::MessageType::Log)) {
          memcpy(&reportToTPC, &dataReceivedFromTestEnv, sizeof(dataReceivedFromTestEnv));
          // send logging messages and final report message to TPC
          sendReport(reportToTPC, dataReceivedFromTPC);
        }
        else {
          // send error message to TPC
          reportToTPC.errorCode = testapp::ErrCode::ERRCODE_MSG_INCOMPLETE;
          reportToTPC.messageType = message::MessageType::Report;
          reportToTPC.subType = message::MessageSubType::TEST_FAIL;
          sendReport(reportToTPC, dataReceivedFromTPC);
        }
      }
      else {
        /*Nothing todo*/
      }
    }
  }

  testapp::ErrCode TestRunner::processTestCmd(const message::MessageStructure &command, message::TestExecutionMessage &dataSentToTestEnv) {
    size_t bytesSentToTestEnv = 0u;
    testapp::ErrCode errCode = testapp::ErrCode::ERRCODE_UNDEFINED;
    testsuite::TestSuiteInterface *testSuitePtr = nullptr;
    testcase::TestCaseInterface *testCasePtr = nullptr;
    // check message subtype, load corresponding testCase or testSuite, and
    // send corresponding information to TestEnv or set an errCode to be sent to TPC
    switch (command.subType) {
      case message::MessageSubType::TESTSUITE_SETUP:
      case message::MessageSubType::TESTSUITE_RUN:
      case message::MessageSubType::TESTSUITE_TEARDOWN:
        // load and get testSuite to be forwarded to testEnv
        testSuitePtr = testrunner::testRegistry.loadTestSuite(command.testSuite);
        if (nullptr != testSuitePtr) {
          dataSentToTestEnv.subType = command.subType;
          dataSentToTestEnv.testSuiteOrCase = testSuitePtr;
          // send message to TestEnv
          bytesSentToTestEnv = _testRunnerToTestEnvironmentMessageBufferReference->send(&dataSentToTestEnv, sizeof(dataSentToTestEnv));
          errCode = (0u != bytesSentToTestEnv) ? testapp::ErrCode::SUCCESS : testapp::ErrCode::FAILURE;
        }
        else{
          errCode = testapp::ErrCode::ERRCODE_TEST_SUITE_UNKNOWN;
        }
        break;
      case message::MessageSubType::TESTCASE_SETUP:
      case message::MessageSubType::TESTCASE_RUN:
      case message::MessageSubType::TESTCASE_TEARDOWN:
        // load and get testCase to be forwarded to testEnv
        testCasePtr = testrunner::testRegistry.loadTestCase(command.testSuite, command.testCase);
        if (nullptr != testCasePtr) {
          dataSentToTestEnv.subType = command.subType;
          dataSentToTestEnv.testSuiteOrCase = testCasePtr;
          // send message to TestEnv
          bytesSentToTestEnv = _testRunnerToTestEnvironmentMessageBufferReference->send(&dataSentToTestEnv, sizeof(dataSentToTestEnv));
          errCode = (0u != bytesSentToTestEnv) ? testapp::ErrCode::SUCCESS : testapp::ErrCode::FAILURE;
        }
        else{
          errCode = testapp::ErrCode::ERRCODE_TEST_CASE_UNKNOWN;
        }
        break;
      default:
        errCode = testapp::ErrCode::FAILURE;
        break;
    }
    return errCode;
  }

  void TestRunner::sendReport(message::MessageStructure &reportMessage, const message::MessageStructure &dataReceivedFromTPC) {
    reportMessage.headerInfo.messageType = (reportMessage.messageType == message::MessageType::Report) ? 1u : 3u;
    reportMessage.headerInfo.protocolVersion = dataReceivedFromTPC.headerInfo.protocolVersion;
    reportMessage.headerInfo.res = dataReceivedFromTPC.headerInfo.res;
    reportMessage.token = dataReceivedFromTPC.token;
    reportMessage.testSection = dataReceivedFromTPC.testSection;
    reportMessage.testSuite = dataReceivedFromTPC.testSuite;
    reportMessage.testCase = dataReceivedFromTPC.testCase;

    _testRunnerToTPCMessageBufferReference->send(&reportMessage, sizeof(reportMessage), portMAX_DELAY);
  }
} // namespace testrunner
