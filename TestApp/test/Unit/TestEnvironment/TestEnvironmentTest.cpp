#include "TestEnvironment/TestEnvironment.hpp"
#include "TestCase/TestCaseInterface.hpp"
#include "TestSuite/TestSuiteInterface.hpp"
#include "MessageBufferReferenceMock/MessageBufferReferenceMock.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <array>

using namespace testapp;

class MockTestCase : public testcase::TestCaseInterface {
public:
  MockTestCase() = default;
  MOCK_METHOD(ErrCode, doSetup, (), (override));
  MOCK_METHOD(ErrCode, doTearDown, (), (override));
  MOCK_METHOD(ErrCode, runTest, (), (override));
};

class MockTestSuite : public testsuite::TestSuiteInterface {
public:
  MockTestSuite() = default;
  MOCK_METHOD(ErrCode, doSuiteSetup, (), (override));
  MOCK_METHOD(ErrCode, doSuiteTearDown, (), (override));
  MOCK_METHOD(testcase::TestCaseInterface*, createTestCase, (testapp::TestIDType testCaseID, void* memoryLocation, size_t memorySize), (override));
};

// Custom action to cast the arg0 of the message buffer receive function to TestExecutionMessage.
ACTION_P(AssignMyType, param) { *static_cast<message::TestExecutionMessage*>(arg0) = param; }

TEST(testEnvironment, test_runOneCycle_testSuite) {
  testenvironment::TestEnvironment testEnvironment;
  MockTestSuite testSuite;
  message::TestExecutionMessage msg;
  msg.testSuiteOrCase = &testSuite;

  // Set up message buffer.
  osextension::MessageBufferReferenceMock TestRunnerToTestEnvMock, TestEnvToTestRunnerMock;
  testEnvironment.registerTestRunnerToTestEnvMessageBufferReference(TestRunnerToTestEnvMock);
  testEnvironment.registerTestEnvToTestRunnerMessageBufferReference(TestEnvToTestRunnerMock);

  // Check call doSuiteSetup.
  msg.subType = message::MessageSubType::TESTSUITE_SETUP;
  EXPECT_CALL(TestRunnerToTestEnvMock, receive(testing::_, sizeof(message::TestExecutionMessage), portMAX_DELAY))
      .WillOnce(testing::DoAll(AssignMyType(msg), testing::Return(sizeof(message::TestExecutionMessage))));
  EXPECT_CALL(TestEnvToTestRunnerMock, send(testing::A<const void*>(), sizeof(message::MessageStructure), testing::_)).Times(1);
  EXPECT_CALL(testSuite, doSuiteSetup()).Times(1);
  testEnvironment.runOneCycle();

  // Check calls doSuiteTearDown.
  msg.subType = message::MessageSubType::TESTSUITE_TEARDOWN;
  EXPECT_CALL(TestRunnerToTestEnvMock, receive(testing::_, sizeof(message::TestExecutionMessage), portMAX_DELAY))
      .WillOnce(testing::DoAll(AssignMyType(msg), testing::Return(sizeof(message::TestExecutionMessage))));
  EXPECT_CALL(TestEnvToTestRunnerMock, send(testing::A<const void*>(), sizeof(message::MessageStructure), testing::_)).Times(1);
  EXPECT_CALL(testSuite, doSuiteTearDown()).Times(1);
  testEnvironment.runOneCycle();
}

TEST(testEnvironment, test_runOneCycle_testCase) {
  testenvironment::TestEnvironment testEnvironment;
  MockTestCase testCase;
  message::TestExecutionMessage msg;
  msg.testSuiteOrCase = &testCase;

  // Set up message buffer.
  osextension::MessageBufferReferenceMock TestRunnerToTestEnvMock, TestEnvToTestRunnerMock;
  testEnvironment.registerTestRunnerToTestEnvMessageBufferReference(TestRunnerToTestEnvMock);
  testEnvironment.registerTestEnvToTestRunnerMessageBufferReference(TestEnvToTestRunnerMock);

  // Check call doSetup.
  msg.subType = message::MessageSubType::TESTCASE_SETUP;
  EXPECT_CALL(TestRunnerToTestEnvMock, receive(testing::_, sizeof(message::TestExecutionMessage), portMAX_DELAY))
      .WillOnce(testing::DoAll(AssignMyType(msg), testing::Return(sizeof(message::TestExecutionMessage))));
  EXPECT_CALL(TestEnvToTestRunnerMock, send(testing::A<const void*>(), sizeof(message::MessageStructure), testing::_)).Times(1);
  EXPECT_CALL(testCase, doSetup()).Times(1);
  testEnvironment.runOneCycle();

  // Check call runTest.
  msg.subType = message::MessageSubType::TESTCASE_RUN;
  EXPECT_CALL(TestRunnerToTestEnvMock, receive(testing::_, sizeof(message::TestExecutionMessage), portMAX_DELAY))
      .WillOnce(testing::DoAll(AssignMyType(msg), testing::Return(sizeof(message::TestExecutionMessage))));
  EXPECT_CALL(TestEnvToTestRunnerMock, send(testing::A<const void*>(), sizeof(message::MessageStructure), testing::_)).Times(1);
  EXPECT_CALL(testCase, runTest()).Times(1);
  testEnvironment.runOneCycle();

  // Check calls doTearDown.
  msg.subType = message::MessageSubType::TESTCASE_TEARDOWN;
  EXPECT_CALL(TestRunnerToTestEnvMock, receive(testing::_, sizeof(message::TestExecutionMessage), portMAX_DELAY))
      .WillOnce(testing::DoAll(AssignMyType(msg), testing::Return(sizeof(message::TestExecutionMessage))));
  EXPECT_CALL(TestEnvToTestRunnerMock, send(testing::A<const void*>(), sizeof(message::MessageStructure), testing::_)).Times(1);
  EXPECT_CALL(testCase, doTearDown()).Times(1);
  testEnvironment.runOneCycle();
}

TEST(testEnvironment, test_runOneCycle_noBuffer) {
  testenvironment::TestEnvironment testEnvironment;
  MockTestSuite testSuite;

  EXPECT_CALL(testSuite, doSuiteSetup()).Times(0);
  testEnvironment.runOneCycle();
}
