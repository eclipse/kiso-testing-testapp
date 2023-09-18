#include <TestEnvironment/Logging.hpp>
#include <MessageBufferReferenceMock/MessageBufferReferenceMock.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::A;
using ::testing::_;

using namespace testapp;

TEST(Logging_TestSuite, Logging_ErrorConditions) {
  osextension::MessageBufferReferenceMock mbuf;

  testenvironment::Logging& Logger = testenvironment::Logging::getInstance();

  ASSERT_EQ(Logger.setMessageBuffer(mbuf), RetCode::RETCODE_SUCCESS);

  const char* testDebugMessage = "This is a test (debug) information. Please ignore";
  const char* testEmptyMessage = "";
  char testTooLongMessage[256];
  int size = 256;
  memset(testTooLongMessage, 'a', size);
  testTooLongMessage[255] = '\0';

  EXPECT_CALL(mbuf, send(_, _, _)).Times(2);

  // Test 1: error conditions
  ASSERT_EQ(Logger.logDebug(nullptr), RetCode::RETCODE_NULL_POINTER);
  ASSERT_EQ(Logger.logDebug(testDebugMessage), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(Logger.setDebugLog(true), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(Logger.logDebug(testEmptyMessage), RetCode::RETCODE_INVALID_PARAM);
  ASSERT_EQ(Logger.logDebug(testTooLongMessage), RetCode::RETCODE_OUT_OF_RESOURCES);
}

TEST(Logging_TestSuite, Logging_DebugMessage50) {
  osextension::MessageBufferReferenceMock mbuf;

  testenvironment::Logging& Logger = testenvironment::Logging::getInstance();

  ASSERT_EQ(Logger.setMessageBuffer(mbuf), RetCode::RETCODE_SUCCESS);

  const char* testDebugMessage = "This is a test (debug) information. Please ignore";
  // Test 2: send debug message with 50 chars
  EXPECT_CALL(mbuf, send(_, _, _)).Times(1);
  ASSERT_EQ(Logger.logDebug(testDebugMessage), RetCode::RETCODE_SUCCESS);
}

TEST(Logging_TestSuite, Logging_DebugMessage100) {
  osextension::MessageBufferReferenceMock mbuf;

  testenvironment::Logging& Logger = testenvironment::Logging::getInstance();

  ASSERT_EQ(Logger.setMessageBuffer(mbuf), RetCode::RETCODE_SUCCESS);

  const char* testLongMessage = "This is a debug message that is too long to be sent in 1 packet. Will be split to several messages.";
  // Test 3: send debug message with 100 chars
  EXPECT_CALL(mbuf, send(_, _, _)).Times(1);
  ASSERT_EQ(Logger.logDebug(testLongMessage), RetCode::RETCODE_SUCCESS);
}

TEST(Logging_TestSuite, Logging_CriticalMessage) {
  osextension::MessageBufferReferenceMock mbuf;

  testenvironment::Logging& Logger = testenvironment::Logging::getInstance();

  ASSERT_EQ(Logger.setMessageBuffer(mbuf), RetCode::RETCODE_SUCCESS);

  const char* testCriticalMessage = "Battery greater 500 degree";
  // send critical message with 27 chars
  EXPECT_CALL(mbuf, send(_, _, _)).Times(2);
  ASSERT_EQ(Logger.logCritical(testCriticalMessage), RetCode::RETCODE_SUCCESS);

  //test with tooLongMessage
  char testTooLongMessage[256];
  int size = 256;
  memset(testTooLongMessage, 'a', size);
  testTooLongMessage[255] = '\0';
  ASSERT_EQ(Logger.logCritical(testTooLongMessage), RetCode::RETCODE_OUT_OF_RESOURCES);
}

TEST(Logging_TestSuite, Logging_logInfo) {
  osextension::MessageBufferReferenceMock mbuf;

  testenvironment::Logging& Logger = testenvironment::Logging::getInstance();

  ASSERT_EQ(Logger.setMessageBuffer(mbuf), RetCode::RETCODE_SUCCESS);

  const char* testDebugMessage = "This is a test (debug) information. Please ignore";

  EXPECT_CALL(mbuf, send(_, _, _)).Times(2);
  ASSERT_EQ(Logger.logInfo(testDebugMessage), RetCode::RETCODE_SUCCESS);

  //test with tooLongMessage
  char testTooLongMessage[256];
  int size = 256;
  memset(testTooLongMessage, 'a', size);
  testTooLongMessage[255] = '\0';
  ASSERT_EQ(Logger.logInfo(testTooLongMessage), RetCode::RETCODE_OUT_OF_RESOURCES);
}

TEST(Logging_TestSuite, Logging_logWarning) {
  osextension::MessageBufferReferenceMock mbuf;

  testenvironment::Logging& Logger = testenvironment::Logging::getInstance();

  ASSERT_EQ(Logger.setMessageBuffer(mbuf), RetCode::RETCODE_SUCCESS);

  const char* testDebugMessage = "This is a test (debug) information. Please ignore";

  EXPECT_CALL(mbuf, send(_, _, _)).Times(2);
  ASSERT_EQ(Logger.logWarning(testDebugMessage), RetCode::RETCODE_SUCCESS);

  //test with tooLongMessage
  char testTooLongMessage[256];
  int size = 256;
  memset(testTooLongMessage, 'a', size);
  testTooLongMessage[255] = '\0';
  ASSERT_EQ(Logger.logWarning(testTooLongMessage), RetCode::RETCODE_OUT_OF_RESOURCES);
}

TEST(Logging_TestSuite, Logging_logError) {
  osextension::MessageBufferReferenceMock mbuf;

  testenvironment::Logging& Logger = testenvironment::Logging::getInstance();

  ASSERT_EQ(Logger.setMessageBuffer(mbuf), RetCode::RETCODE_SUCCESS);

  const char* testDebugMessage = "This is a test (debug) information. Please ignore";

  EXPECT_CALL(mbuf, send(_, _, _)).Times(2);
  ASSERT_EQ(Logger.logError(testDebugMessage), RetCode::RETCODE_SUCCESS);

  //test with tooLongMessage
  char testTooLongMessage[256];
  int size = 256;
  memset(testTooLongMessage, 'a', size);
  testTooLongMessage[255] = '\0';
  ASSERT_EQ(Logger.logError(testTooLongMessage), RetCode::RETCODE_OUT_OF_RESOURCES);
}

TEST(Logging_TestSuite, Logging_logMessage) {
  osextension::MessageBufferReferenceMock mbuf;

  testenvironment::Logging& Logger = testenvironment::Logging::getInstance();

  ASSERT_EQ(Logger.setMessageBuffer(mbuf), RetCode::RETCODE_SUCCESS);

  const char* testDebugMessage = "This is a test (debug) information. Please ignore";

  EXPECT_CALL(mbuf, send(_, _, _)).Times(2);
  ASSERT_EQ(Logger.logMessage(testDebugMessage), RetCode::RETCODE_SUCCESS);

  //test with tooLongMessage
  char testTooLongMessage[256];
  int size = 256;
  memset(testTooLongMessage, 'a', size);
  testTooLongMessage[255] = '\0';
  ASSERT_EQ(Logger.logMessage(testTooLongMessage), RetCode::RETCODE_OUT_OF_RESOURCES);
}
