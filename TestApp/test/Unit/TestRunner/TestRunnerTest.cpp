#include <OsAbstraction/OsAbstraction.hpp>
#include <FreeRtosMock.hpp>

#include <TestRunner/TestRunner.hpp>
#include <MessageBufferReferenceMock/MessageBufferReferenceMock.hpp>

#include <TestRegistry/TestRegistry.hpp>
#include "testSuite1.cpp"

#include <cstring> // std::memcmp
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::A;
using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::DoAll;

// Test fixture class
class TestRunnerTest : public testing::Test
{
public:
  osextension::MessageBufferReferenceMock TestEnvironmentToTestRunnerMessageBuffer;
  osextension::MessageBufferReferenceMock TPCToTestRunnerMessageBuffer;
  osextension::MessageBufferReferenceMock TestRunnerToTestEnvironmentMessageBuffer;
  osextension::MessageBufferReferenceMock TestRunnerToTPCMessageBuffer;

  QueueSetMemberHandle_t queueSetMemberHandle;
  testrunner::TestRunner testRunner{};
  message::TestExecutionMessage dataSentToTestEnv{};
  message::MessageStructure dataReceivedFromTestEnv{};
  message::MessageStructure dataReceivedFromTPC{};
  message::MessageStructure reportToTPC{};
  testapp::ErrCode retErrCode;

  virtual void SetUp()
  {
    freertos::mock = std::make_unique<freertos::Mock>();
    testRunner.setTestEnvironmentToTestRunnerMessageBufferReference(TestEnvironmentToTestRunnerMessageBuffer);
    testRunner.setTPCToTestRunnerMessageBufferReference(TPCToTestRunnerMessageBuffer);
    testRunner.setTestRunnerToTestEnvironmentMessageBufferReference(TestRunnerToTestEnvironmentMessageBuffer);
    testRunner.setTestRunnerToTPCMessageBufferReference(TestRunnerToTPCMessageBuffer);

    dataSentToTestEnv.subType = message::MessageSubType::TEST_NOTIMPLEMENTED;
    dataSentToTestEnv.testSuiteOrCase = nullptr;
    retErrCode = testapp::ErrCode::ERRCODE_UNDEFINED;
  }

  virtual void TearDown()
  {
    freertos::mock.reset(nullptr);
  }
};

TEST_F(TestRunnerTest, processTestCmdTestSuiteSetup)
{
  // Input data
  message::MessageStructure command = {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTSUITE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 0u, {}};
  // expected returned code
  testapp::ErrCode expectedErrCode = testapp::ErrCode::SUCCESS;
  // expected Message subType to be sent to TestEnvironment
  message::MessageSubType expectedMessageSubType = message::MessageSubType::TESTSUITE_SETUP;

  EXPECT_CALL(TestRunnerToTestEnvironmentMessageBuffer, send(A<const void*>(), _)).Times(1).WillOnce(Return(sizeof(dataSentToTestEnv)));

  retErrCode = testRunner.processTestCmd(command, dataSentToTestEnv);
  EXPECT_EQ(expectedErrCode, retErrCode);
  EXPECT_EQ(expectedMessageSubType, dataSentToTestEnv.subType);

  // check pointer to TestSuite Or TestCase
  EXPECT_NE((void *)0, dataSentToTestEnv.testSuiteOrCase);
}

TEST_F(TestRunnerTest, processTestCmdTestCaseSetup)
{
  // Input data
  message::MessageStructure command = {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTCASE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, 0u, {}};
  // expected returned code
  testapp::ErrCode expectedErrCode = testapp::ErrCode::SUCCESS;
  // expected Message subType to be sent to TestEnvironment
  message::MessageSubType expectedMessageSubType = message::MessageSubType::TESTCASE_SETUP;

  EXPECT_CALL(TestRunnerToTestEnvironmentMessageBuffer, send(A<const void*>(), _)).Times(1).WillOnce(Return(sizeof(dataSentToTestEnv)));

  retErrCode = testRunner.processTestCmd(command, dataSentToTestEnv);
  EXPECT_EQ(expectedErrCode, retErrCode);
  EXPECT_EQ(expectedMessageSubType, dataSentToTestEnv.subType);

  // check pointer to TestSuite Or TestCase
  EXPECT_NE((void *)0, dataSentToTestEnv.testSuiteOrCase);
}

TEST_F(TestRunnerTest, processTestCmdTestSuiteRun)
{
  // Input data
  message::MessageStructure command = {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTSUITE_RUN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 0u, {}};
  // expected returned code
  testapp::ErrCode expectedErrCode = testapp::ErrCode::SUCCESS;
  // expected Message subType to be sent to TestEnvironment
  message::MessageSubType expectedMessageSubType = message::MessageSubType::TESTSUITE_RUN;

  EXPECT_CALL(TestRunnerToTestEnvironmentMessageBuffer, send(A<const void*>(), _)).Times(1).WillOnce(Return(sizeof(dataSentToTestEnv)));

  retErrCode = testRunner.processTestCmd(command, dataSentToTestEnv);
  EXPECT_EQ(expectedErrCode, retErrCode);
  EXPECT_EQ(expectedMessageSubType, dataSentToTestEnv.subType);

  // check pointer to TestSuite Or TestCase
  EXPECT_NE((void *)0, dataSentToTestEnv.testSuiteOrCase);
}

TEST_F(TestRunnerTest, processTestCmdTestCaseRun)
{
  // Input data
  message::MessageStructure command = {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTCASE_RUN, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, 0u, {}};
  // expected returned code
  testapp::ErrCode expectedErrCode = testapp::ErrCode::SUCCESS;
  // expected Message subType to be sent to TestEnvironment
  message::MessageSubType expectedMessageSubType = message::MessageSubType::TESTCASE_RUN;

  EXPECT_CALL(TestRunnerToTestEnvironmentMessageBuffer, send(A<const void*>(), _)).Times(1).WillOnce(Return(sizeof(dataSentToTestEnv)));

  retErrCode = testRunner.processTestCmd(command, dataSentToTestEnv);
  EXPECT_EQ(expectedErrCode, retErrCode);
  EXPECT_EQ(expectedMessageSubType, dataSentToTestEnv.subType);

  // check pointer to TestSuite Or TestCase
  EXPECT_NE((void *)0, dataSentToTestEnv.testSuiteOrCase);
}

TEST_F(TestRunnerTest, processTestCmdTestCaseRunTestSuiteTearDown)
{
  // Input data
  message::MessageStructure command = {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTSUITE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 0u, {}};
  // expected returned code
  testapp::ErrCode expectedErrCode = testapp::ErrCode::SUCCESS;
  // expected Message subType to be sent to TestEnvironment
  message::MessageSubType expectedMessageSubType = message::MessageSubType::TESTSUITE_TEARDOWN;

  EXPECT_CALL(TestRunnerToTestEnvironmentMessageBuffer, send(A<const void*>(), _)).Times(1).WillOnce(Return(sizeof(dataSentToTestEnv)));

  retErrCode = testRunner.processTestCmd(command, dataSentToTestEnv);
  EXPECT_EQ(expectedErrCode, retErrCode);
  EXPECT_EQ(expectedMessageSubType, dataSentToTestEnv.subType);

  // check pointer to TestSuite Or TestCase
  EXPECT_NE((void *)0, dataSentToTestEnv.testSuiteOrCase);
}

TEST_F(TestRunnerTest, processTestCmdTestCaseRunTestCaseTearDown)
{
  // Input data
  message::MessageStructure command = {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTCASE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, 0u, {}};
  // expected returned code
  testapp::ErrCode expectedErrCode = testapp::ErrCode::SUCCESS;
  // expected Message subType to be sent to TestEnvironment
  message::MessageSubType expectedMessageSubType = message::MessageSubType::TESTCASE_TEARDOWN;

  EXPECT_CALL(TestRunnerToTestEnvironmentMessageBuffer, send(A<const void*>(), _)).Times(1).WillOnce(Return(sizeof(dataSentToTestEnv)));

  retErrCode = testRunner.processTestCmd(command, dataSentToTestEnv);
  EXPECT_EQ(expectedErrCode, retErrCode);
  EXPECT_EQ(expectedMessageSubType, dataSentToTestEnv.subType);

  // check pointer to TestSuite Or TestCase
  EXPECT_NE((void *)0, dataSentToTestEnv.testSuiteOrCase);
}

TEST_F(TestRunnerTest, processTestCmdWrongMessageSubType)
{
  // Input data
  message::MessageStructure command = {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::ACKNOWLEDGEMENT, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, 0u, {}};
  // expected returned code
  testapp::ErrCode expectedErrCode = testapp::ErrCode::FAILURE;

  EXPECT_CALL(TestRunnerToTestEnvironmentMessageBuffer, send(A<const void*>(), _)).Times(0);

  retErrCode = testRunner.processTestCmd(command, dataSentToTestEnv);
  EXPECT_EQ(expectedErrCode, retErrCode);
}

TEST_F(TestRunnerTest, processTestCmdUndefinedTestSuite)
{
  // Input data
  message::MessageStructure command = {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTSUITE_SETUP, testapp::ErrCode::SUCCESS, 0u, 3u, 1u, 0u, {}};
  // expected returned code
  testapp::ErrCode expectedErrCode = testapp::ErrCode::ERRCODE_TEST_SUITE_UNKNOWN;

  EXPECT_CALL(TestRunnerToTestEnvironmentMessageBuffer, send(A<const void*>(), _)).Times(0);

  retErrCode = testRunner.processTestCmd(command, dataSentToTestEnv);
  EXPECT_EQ(expectedErrCode, retErrCode);

  // check pointer to TestSuite Or TestCase
  EXPECT_EQ((void *)0, dataSentToTestEnv.testSuiteOrCase);
}

TEST_F(TestRunnerTest, processTestCmdUndefinedTestCase)
{
  // Input data
  message::MessageStructure command = {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTCASE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 3u, 0u, {}};
  // expected returned code
  testapp::ErrCode expectedErrCode = testapp::ErrCode::ERRCODE_TEST_CASE_UNKNOWN;

  EXPECT_CALL(TestRunnerToTestEnvironmentMessageBuffer, send(A<const void*>(), _)).Times(0);

  retErrCode = testRunner.processTestCmd(command, dataSentToTestEnv);
  EXPECT_EQ(expectedErrCode, retErrCode);

  // check pointer to TestSuite Or TestCase
  EXPECT_EQ((void *)0, dataSentToTestEnv.testSuiteOrCase);
}

TEST_F(TestRunnerTest, writeLogTest)
{
  const uint8_t nbTestCases = 6u;

  // Input Data
  testapp::LogMessageT _logsFromTestEnv[nbTestCases] = {
      {testapp::MessageType::MESSAGE_INFO, "INFO: process test TESTCASE_TEARDOWN"},
      {testapp::MessageType::MESSAGE_DEBUG, "DEBUG: process test TESTSUITE_SETUP"},
      {testapp::MessageType::MESSAGE_WARNING, "WARNING: process test TESTSUITE_TEARDOWN"},
      {testapp::MessageType::MESSAGE_ERROR, "ERROR: process test suite TESTCASE_SETUP"},
      {testapp::MessageType::MESSAGE_CRITICAL, "CRITICAL: process test TESTCASE_RUN"},
      {testapp::MessageType::MESSAGE_LOG, "LOG: process test TESTCASE_TEARDOWN"}};

  message::MessageStructure _dataReceivedFromTestEnv{};

  message::MessageStructure _dataReceivedFromTPC[nbTestCases] = {
      {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTCASE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, 0u, {}},
      {{1, 0, 0}, message::MessageType::Command, 1u, message::MessageSubType::TESTSUITE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 0u, {}},
      {{2, 0, 0}, message::MessageType::Command, 2u, message::MessageSubType::TESTSUITE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 0u, {}},
      {{0, 0, 0}, message::MessageType::Command, 3u, message::MessageSubType::TESTCASE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 2u, 0u, {}},
      {{1, 0, 0}, message::MessageType::Command, 4u, message::MessageSubType::TESTCASE_RUN, testapp::ErrCode::SUCCESS, 0u, 1u, 2u, 0u, {}},
      {{2, 0, 0}, message::MessageType::Command, 5u, message::MessageSubType::TESTCASE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, 0u, {}}};

  // expected report
  message::MessageStructure expectedLogMessage[nbTestCases] = {
      {{0, 3, 0}, message::MessageType::Log, 0u, message::MessageSubType::TESTCASE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(_logsFromTestEnv[0].text.data()))))), {}},
      {{1, 3, 0}, message::MessageType::Log, 1u, message::MessageSubType::TESTSUITE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(_logsFromTestEnv[1].text.data()))))), {}},
      {{2, 3, 0}, message::MessageType::Log, 2u, message::MessageSubType::TESTSUITE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(_logsFromTestEnv[2].text.data()))))), {}},
      {{0, 3, 0}, message::MessageType::Log, 3u, message::MessageSubType::TESTCASE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 2u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(_logsFromTestEnv[3].text.data()))))), {}},
      {{1, 3, 0}, message::MessageType::Log, 4u, message::MessageSubType::TESTCASE_RUN, testapp::ErrCode::SUCCESS, 0u, 1u, 2u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(_logsFromTestEnv[4].text.data()))))), {}},
      {{2, 3, 0}, message::MessageType::Log, 5u, message::MessageSubType::TESTCASE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(_logsFromTestEnv[5].text.data()))))), {}}};

  for(uint8_t iter = 0; iter < nbTestCases; iter++)
  {
    // set input data
    _dataReceivedFromTestEnv.messageType = message::MessageType::Log;
    _dataReceivedFromTestEnv.payloadLength = (uint8_t)strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(_logsFromTestEnv[iter].text.data()))));
    memcpy(_dataReceivedFromTestEnv.payload.data(),
        _logsFromTestEnv[iter].text.data(),
        _dataReceivedFromTestEnv.payload.size());

    dataReceivedFromTestEnv = _dataReceivedFromTestEnv;
    dataReceivedFromTPC = _dataReceivedFromTPC[iter];

    EXPECT_CALL(TestRunnerToTPCMessageBuffer, send(A<const void*>(), _, _)).Times(1).WillOnce(Return(sizeof(dataReceivedFromTestEnv)));

    // function call
    testRunner.sendReport(dataReceivedFromTestEnv, dataReceivedFromTPC);

    EXPECT_TRUE(0 == memcmp(&expectedLogMessage[iter].headerInfo, &dataReceivedFromTestEnv.headerInfo, sizeof(dataReceivedFromTestEnv.headerInfo)));
    EXPECT_EQ(expectedLogMessage[iter].errorCode, dataReceivedFromTestEnv.errorCode);
    EXPECT_EQ(expectedLogMessage[iter].messageType, dataReceivedFromTestEnv.messageType);
    EXPECT_EQ(expectedLogMessage[iter].testSuite, dataReceivedFromTestEnv.testSuite);
    EXPECT_EQ(expectedLogMessage[iter].testCase, dataReceivedFromTestEnv.testCase);
    EXPECT_EQ(expectedLogMessage[iter].token, dataReceivedFromTestEnv.token);
    EXPECT_EQ(expectedLogMessage[iter].payloadLength, dataReceivedFromTestEnv.payloadLength);
    EXPECT_TRUE(0 == memcmp(&dataReceivedFromTestEnv.payload[0], _logsFromTestEnv[iter].text.data(), sizeof(dataReceivedFromTestEnv.payload.size())));
  }
}

TEST_F(TestRunnerTest, reportTest)
{
  const uint8_t nbTestCases = 6u;

  // Input Data
  testapp::LogMessageT _logsFromTestEnv[nbTestCases] = {
      {testapp::MessageType::MESSAGE_INFO, "INFO: process test TESTCASE_TEARDOWN"},
      {testapp::MessageType::MESSAGE_DEBUG, "DEBUG: process test TESTSUITE_SETUP"},
      {testapp::MessageType::MESSAGE_WARNING, "WARNING: process test TESTSUITE_TEARDOWN"},
      {testapp::MessageType::MESSAGE_ERROR, "ERROR: process test suite TESTCASE_SETUP"},
      {testapp::MessageType::MESSAGE_CRITICAL, "CRITICAL: process test TESTCASE_RUN"},
      {testapp::MessageType::MESSAGE_LOG, "LOG: process test TESTCASE_TEARDOWN"}};

   message::MessageStructure _MessageFromTPC[nbTestCases] = {
      {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTCASE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, 0u, {}},
      {{1, 0, 0}, message::MessageType::Command, 1u, message::MessageSubType::TESTSUITE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 0u, {}},
      {{2, 0, 0}, message::MessageType::Command, 2u, message::MessageSubType::TESTSUITE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 0u, {}},
      {{0, 0, 0}, message::MessageType::Command, 3u, message::MessageSubType::TESTCASE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 2u, 0u, {}},
      {{1, 0, 0}, message::MessageType::Command, 4u, message::MessageSubType::TESTCASE_RUN, testapp::ErrCode::SUCCESS, 0u, 1u, 2u, 0u, {}},
      {{2, 0, 0}, message::MessageType::Command, 5u, message::MessageSubType::TESTCASE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, 0u, {}}};


  message::MessageStructure _ReportMessage[nbTestCases] = {
      {{0, 1, 0}, message::MessageType::Report, 0u, message::MessageSubType::TESTCASE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(_logsFromTestEnv[0].text.data())))) + 2), {}},
      {{1, 1, 0}, message::MessageType::Report, 1u, message::MessageSubType::TESTSUITE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(_logsFromTestEnv[1].text.data())))) + 2), {}},
      {{2, 1, 0}, message::MessageType::Report, 2u, message::MessageSubType::TESTSUITE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(_logsFromTestEnv[2].text.data())))) + 2), {}},
      {{0, 1, 0}, message::MessageType::Report, 3u, message::MessageSubType::TESTCASE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 2u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(_logsFromTestEnv[3].text.data())))) + 2), {}},
      {{1, 1, 0}, message::MessageType::Report, 4u, message::MessageSubType::TESTCASE_RUN, testapp::ErrCode::SUCCESS, 0u, 1u, 2u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(_logsFromTestEnv[4].text.data())))) + 2), {}},
      {{2, 1, 0}, message::MessageType::Report, 5u, message::MessageSubType::TESTCASE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(_logsFromTestEnv[5].text.data())))) + 2), {}}};

  for(uint8_t iter = 0; iter < nbTestCases; iter++)
  {
    // set input data
    reportToTPC = _ReportMessage[iter];
    dataReceivedFromTPC = _MessageFromTPC[iter];

    EXPECT_CALL(TestRunnerToTPCMessageBuffer, send(A<const void*>(), _, _)).Times(1).WillOnce(Return(sizeof(reportToTPC)));

    // function call
    testRunner.sendReport(reportToTPC, dataReceivedFromTPC);
  }
}

ACTION_P(MessageStructureType, param) { *static_cast<message::MessageStructure*>(arg0) = param; }

TEST_F(TestRunnerTest, oneCycleTest_TPC_Runner_Env)
{
  // Input data
  dataReceivedFromTPC = {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTSUITE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 0u, {}};

  uint8_t fakeHandle = 1u;
  QueueHandle_t fakeQueueHandleOne = (QueueHandle_t)&fakeHandle;

  EXPECT_CALL(TPCToTestRunnerMessageBuffer, getHandleForQueueSet()).WillRepeatedly(Return(fakeQueueHandleOne));

  EXPECT_CALL(*freertos::mock, xQueueSelectFromSet(_, _)).WillOnce(Return(fakeQueueHandleOne));
  EXPECT_CALL(TPCToTestRunnerMessageBuffer, receive(_, sizeof(dataReceivedFromTPC), portMAX_DELAY))
  .WillOnce(DoAll(MessageStructureType(dataReceivedFromTPC), Return(sizeof(dataReceivedFromTPC))));

  EXPECT_CALL(TestRunnerToTestEnvironmentMessageBuffer, send(A<const void*>(), _)).Times(1).WillOnce(Return(sizeof(dataSentToTestEnv)));

  // function call
  testRunner.oneCycle(queueSetMemberHandle);
}

TEST_F(TestRunnerTest, oneCycleTest_TPC_Runner_TPC)
{
  // Input data
  dataReceivedFromTPC = {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTSUITE_SETUP, testapp::ErrCode::SUCCESS, 0u, 2u, 0u, 0u, {}};

  uint8_t fakeHandle = 1u;
  QueueHandle_t fakeQueueHandleOne = (QueueHandle_t)&fakeHandle;

  EXPECT_CALL(TPCToTestRunnerMessageBuffer, getHandleForQueueSet()).WillRepeatedly(Return(fakeQueueHandleOne));

  EXPECT_CALL(*freertos::mock, xQueueSelectFromSet(_, _)).WillOnce(Return(fakeQueueHandleOne));
  EXPECT_CALL(TPCToTestRunnerMessageBuffer, receive(_, sizeof(dataReceivedFromTPC), portMAX_DELAY))
  .WillOnce(DoAll(MessageStructureType(dataReceivedFromTPC), Return(sizeof(dataReceivedFromTPC))));

  EXPECT_CALL(TestRunnerToTPCMessageBuffer, send(A<const void*>(), _, _)).Times(1).WillOnce(Return(sizeof(dataSentToTestEnv)));

  // function call
  testRunner.oneCycle(queueSetMemberHandle);
}

ACTION_P(MessageFromTestEnvironmentType, param) { *static_cast<message::MessageStructure*>(arg0) = param; }

TEST_F(TestRunnerTest, oneCycleTest_Env_Runner_TPC)
{
  // Input data (Log from TestEnvironment)
  testapp::LogMessageT _logsFromTestEnv{testapp::MessageType::MESSAGE_INFO, "INFO: process test TESTCASE_TEARDOWN"};

  dataReceivedFromTestEnv = {{0, 3, 0}, message::MessageType::Log, 0u, message::MessageSubType::RESERVED, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 0u, {}};
  dataReceivedFromTestEnv.payloadLength = sizeof(_logsFromTestEnv.text);
  std::copy(&_logsFromTestEnv.text[0], &_logsFromTestEnv.text[0] + sizeof(_logsFromTestEnv.text),dataReceivedFromTestEnv.payload.data());

  uint8_t fakeHandle = 1u;
  QueueHandle_t fakeQueueHandleOne = (QueueHandle_t)&fakeHandle;

  EXPECT_CALL(TPCToTestRunnerMessageBuffer, getHandleForQueueSet()).WillRepeatedly(Return(nullptr));
  EXPECT_CALL(TestEnvironmentToTestRunnerMessageBuffer, getHandleForQueueSet()).WillRepeatedly(Return(fakeQueueHandleOne));

  EXPECT_CALL(*freertos::mock, xQueueSelectFromSet(_, _)).WillOnce(Return(fakeQueueHandleOne));
  EXPECT_CALL(TestEnvironmentToTestRunnerMessageBuffer, receive(_, sizeof(dataReceivedFromTestEnv), portMAX_DELAY))
  .WillOnce(DoAll(MessageFromTestEnvironmentType(dataReceivedFromTestEnv), Return(sizeof(dataReceivedFromTestEnv))));

  EXPECT_CALL(TestRunnerToTPCMessageBuffer, send(A<const void*>(), _, _)).Times(1).WillOnce(Return(sizeof(reportToTPC)));

  // function call
  testRunner.oneCycle(queueSetMemberHandle);
}

TEST_F(TestRunnerTest, oneCycleTest_nullHandleForQueueSet)
{
  EXPECT_CALL(TPCToTestRunnerMessageBuffer, getHandleForQueueSet()).WillRepeatedly(Return(nullptr));
  EXPECT_CALL(TestEnvironmentToTestRunnerMessageBuffer, getHandleForQueueSet()).WillRepeatedly(nullptr);
  EXPECT_CALL(*freertos::mock, xQueueSelectFromSet(_, _)).WillOnce(Return(nullptr));

  // function call
  testRunner.oneCycle(queueSetMemberHandle);
}
