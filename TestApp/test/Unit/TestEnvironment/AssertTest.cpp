/**
 * @file test/Unit/TestEnvironment/AssertTest.cpp
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
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <TestEnvironment/Assert.hpp>
#include <TestEnvironment/Logging.hpp>

using namespace testing;
using namespace testapp;
using namespace testenvironment;

class MockLogger : public LoggingInterface {
public:
  MockLogger() = default;
  MOCK_METHOD(RetCode, logInfo, (const char*), (override));
  MOCK_METHOD(RetCode, logDebug, (const char*), (override));
  MOCK_METHOD(RetCode, logWarning, (const char*), (override));
  MOCK_METHOD(RetCode, logError, (const char*), (override));
  MOCK_METHOD(RetCode, logCritical, (const char*), (override));
  MOCK_METHOD(RetCode, logMessage, (const char*), (override));
  MOCK_METHOD(void, resetFailureFlag, (),  (override));
  MOCK_METHOD(bool, getFailureFlag, (),  (override));
};

TEST(Assert_TestSuite, test_unititializedLogger) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;

  // Test assert_streq when Assert is not initialized
  ASSERT_EQ(assert.TA_ASSERT_STREQ("abc", "abc"), RetCode::RETCODE_UNINITIALIZED);

  // Test logAssert() with uninitialized logger
  {
    AssertReport AssertReport(AssertTypes::ASSERT_GE, 1.0, 1.0, RetCode::RETCODE_SUCCESS, 5, "testfile");
    ASSERT_EQ(assert.logAssert(AssertReport), RetCode::RETCODE_UNINITIALIZED);
  }

  // Test returnReportAndReset() with uninitialized logger
  {
    AssertReport AssertReport(AssertTypes::ASSERT_GE, 1.0, 1.0, RetCode::RETCODE_SUCCESS, 5, "testfile");
    ASSERT_EQ(assert.logAssert(AssertReport), RetCode::RETCODE_UNINITIALIZED);
    ASSERT_EQ(assert.returnReportAndReset(), ReportResult::ReportFailure);
  }
}

TEST(Assert_TestSuite, test_setLogger) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;
  ASSERT_EQ(assert.setLogger(mocklogger), RetCode::RETCODE_SUCCESS);
}

TEST(Assert_TestSuite, test_logAssert) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;
  assert.setLogger(mocklogger);

  // Test with oversized fileName (leaves not enough room for full report)
  {
    char oversizedFilename[250];
    for (uint16_t i = 0; i < 250; i++) {
      oversizedFilename[i] = 'x';
    }
    oversizedFilename[249] = '\0';
    AssertReport AssertReport(AssertTypes::ASSERT_GE, 1.0, 1.0, RetCode::RETCODE_SUCCESS, 5, oversizedFilename);
    ASSERT_EQ(assert.logAssert(AssertReport), RetCode::RETCODE_UNEXPECTED_BEHAVIOR);
  }
  // Tets successful case
  {
    AssertReport AssertReport(AssertTypes::ASSERT_GE, 1.0, 1.0, RetCode::RETCODE_SUCCESS, 5, "testfile");
    ASSERT_EQ(assert.logAssert(AssertReport), RetCode::RETCODE_SUCCESS);
  }
}

TEST(Assert_TestSuite, Assert_assert_true) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;
  assert.setLogger(mocklogger);

  EXPECT_CALL(mocklogger, logError(HasSubstr("Success ASSERT_TRUE input: true"))).Times(0);
  EXPECT_CALL(mocklogger, logError(HasSubstr("Failure ASSERT_TRUE input: false"))).Times(2);

  // Test equal
  ASSERT_EQ(assert.TA_ASSERT_TRUE(2), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_TRUE(2.0f), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_TRUE(0.0f), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_TRUE(true), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_TRUE(false), RetCode::RETCODE_FAILURE);
}

TEST(Assert_TestSuite, Assert_assert_false) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;
  assert.setLogger(mocklogger);

  EXPECT_CALL(mocklogger, logError(HasSubstr("Success ASSERT_FALSE input: false"))).Times(0);
  EXPECT_CALL(mocklogger, logError(HasSubstr("Failure ASSERT_FALSE input: true"))).Times(3);

  ASSERT_EQ(assert.TA_ASSERT_FALSE(2), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_FALSE(2.0f), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_FALSE(0.0f), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_FALSE(true), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_FALSE(false), RetCode::RETCODE_SUCCESS);
}

TEST(Assert_TestSuite, Assert_assert_eq) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;
  assert.setLogger(mocklogger);

  EXPECT_CALL(mocklogger, logError(HasSubstr("Success ASSERT_EQ"))).Times(0);
  EXPECT_CALL(mocklogger, logError(HasSubstr("Failure ASSERT_EQ"))).Times(2);

  // Test equal
  ASSERT_EQ(assert.TA_ASSERT_EQ(2, 3), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_EQ(1, 1), RetCode::RETCODE_SUCCESS);

  ASSERT_EQ(assert.TA_ASSERT_EQ(42UL, 42UL), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_EQ(42UL, 56UL), RetCode::RETCODE_FAILURE);
}

TEST(Assert_TestSuite, test_assert_ne) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;
  assert.setLogger(mocklogger);

  EXPECT_CALL(mocklogger, logError(HasSubstr("Success ASSERT_NE"))).Times(0);
  EXPECT_CALL(mocklogger, logError(HasSubstr("Failure ASSERT_NE"))).Times(2);

  // Test not equal
  ASSERT_EQ(assert.TA_ASSERT_NE(5, 6), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_NE(1, 1), RetCode::RETCODE_FAILURE);

  ASSERT_EQ(assert.TA_ASSERT_NE(42UL, 56UL), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_NE(5UL, 5UL), RetCode::RETCODE_FAILURE);
}

TEST(Assert_TestSuite, test_assert_lt) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;
  assert.setLogger(mocklogger);

  EXPECT_CALL(mocklogger, logError(HasSubstr("Success ASSERT_LT"))).Times(0);
  EXPECT_CALL(mocklogger, logError(HasSubstr("Failure ASSERT_LT"))).Times(4);

  // Test less than
  ASSERT_EQ(assert.TA_ASSERT_LT(255, 2), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_LT(3, 5), RetCode::RETCODE_SUCCESS);

  ASSERT_EQ(assert.TA_ASSERT_LT(42UL, 42UL), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_LT(24UL, 32UL), RetCode::RETCODE_SUCCESS);

  ASSERT_EQ(assert.TA_ASSERT_LT(2.9f, 3.0f), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_LT(0.0f, 0.0f), RetCode::RETCODE_FAILURE);

  ASSERT_EQ(assert.TA_ASSERT_LT(-128, -3), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_LT(-127, -128), RetCode::RETCODE_FAILURE);
}

TEST(Assert_TestSuite, test_assert_le) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;
  assert.setLogger(mocklogger);

  EXPECT_CALL(mocklogger, logError(HasSubstr("Success ASSERT_LE"))).Times(0);
  EXPECT_CALL(mocklogger, logError(HasSubstr("Failure ASSERT_LE"))).Times(4);

  // Test less than or equal
  ASSERT_EQ(assert.TA_ASSERT_LE(0, 0), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_LE(5, 7), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_LE(255, 3), RetCode::RETCODE_FAILURE);

  ASSERT_EQ(assert.TA_ASSERT_LE(56UL, 56UL), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_LE(56UL, 48UL), RetCode::RETCODE_FAILURE);

  ASSERT_EQ(assert.TA_ASSERT_LE(3.9f, 4.0f), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_LE(9.0f, 3.0f), RetCode::RETCODE_FAILURE);

  ASSERT_EQ(assert.TA_ASSERT_LE(-128, -7), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_LE(-3, -5), RetCode::RETCODE_FAILURE);
}

TEST(Assert_TestSuite, test_assert_gt) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;
  assert.setLogger(mocklogger);

  EXPECT_CALL(mocklogger, logError(HasSubstr("Success ASSERT_GT"))).Times(0);
  EXPECT_CALL(mocklogger, logError(HasSubstr("Failure ASSERT_GT"))).Times(12);

  // Test greater then
  ASSERT_EQ(assert.TA_ASSERT_GT(2.000, 2.001), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GT(2.000, 2.000), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GT(2.000, 1.999), RetCode::RETCODE_SUCCESS);

  ASSERT_EQ(assert.TA_ASSERT_GT(-2.000, -1.999), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GT(-2.000, -2.000), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GT(-2.000, -2.001), RetCode::RETCODE_SUCCESS);

  ASSERT_EQ(assert.TA_ASSERT_GT(0.000, 0.001), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GT(0.000, 0.000), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GT(0.000, -0.001), RetCode::RETCODE_SUCCESS);

  int32_t longMinValue = std::numeric_limits<std::int32_t>::min();
  int32_t longMaxValue = std::numeric_limits<std::int32_t>::max();
  int32_t longZeroValue = 0;

  ASSERT_EQ(assert.TA_ASSERT_GT(longMaxValue - 1, longMaxValue), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GT(longMaxValue, longMaxValue), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GT(longMaxValue, longMaxValue - 1), RetCode::RETCODE_SUCCESS);

  ASSERT_EQ(assert.TA_ASSERT_GT(longMinValue, longMinValue + 1), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GT(longMinValue, longMinValue), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GT(longMinValue + 1, longMinValue), RetCode::RETCODE_SUCCESS);

  ASSERT_EQ(assert.TA_ASSERT_GT(longZeroValue, longZeroValue + 1), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GT(longZeroValue, longZeroValue), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GT(longZeroValue, longZeroValue - 1), RetCode::RETCODE_SUCCESS);
}

TEST(Assert_TestSuite, test_assert_ge) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;
  assert.setLogger(mocklogger);

  EXPECT_CALL(mocklogger, logError(HasSubstr("Success ASSERT_GE"))).Times(0);
  EXPECT_CALL(mocklogger, logError(HasSubstr("Failure ASSERT_GE"))).Times(6);

  // Test greater equal
  ASSERT_EQ(assert.TA_ASSERT_GE(2.000, 2.001), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GE(2.000, 2.000), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_GE(2.000, 1.999), RetCode::RETCODE_SUCCESS);

  ASSERT_EQ(assert.TA_ASSERT_GE(-2.000, -1.999), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GE(-2.000, -2.000), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_GE(-2.000, -2.001), RetCode::RETCODE_SUCCESS);

  ASSERT_EQ(assert.TA_ASSERT_GE(0.000, 0.001), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GE(0.000, 0.000), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_GE(0.000, -0.001), RetCode::RETCODE_SUCCESS);

  int32_t longMinValue = std::numeric_limits<std::int32_t>::min();
  int32_t longMaxValue = std::numeric_limits<std::int32_t>::max();
  int32_t longZeroValue = 0;

  ASSERT_EQ(assert.TA_ASSERT_GE(longMaxValue - 1, longMaxValue), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GE(longMaxValue, longMaxValue), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_GE(longMaxValue, longMaxValue - 1), RetCode::RETCODE_SUCCESS);

  ASSERT_EQ(assert.TA_ASSERT_GE(longMinValue, longMinValue + 1), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GE(longMinValue, longMinValue), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_GE(longMinValue + 1, longMinValue), RetCode::RETCODE_SUCCESS);

  ASSERT_EQ(assert.TA_ASSERT_GE(longZeroValue, longZeroValue + 1), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_GE(longZeroValue, longZeroValue), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_GE(longZeroValue, longZeroValue - 1), RetCode::RETCODE_SUCCESS);
}

TEST(Assert_TestSuite, test_assert_streq) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;
  assert.setLogger(mocklogger);

  EXPECT_CALL(mocklogger, logError(HasSubstr("Success"))).Times(0);
  // Ensure, that different string variables are passed as arguments to the
  // assert Trace function by explicitly checking one complete failed string
  EXPECT_CALL(mocklogger, logError(HasSubstr("Failure ASSERT_STREQ input 1: ab input 2: abc"))).Times(1);
  EXPECT_CALL(mocklogger, logError(HasSubstr("Failure ASSERT_STREQ input 1: abc"))).Times(3);
  EXPECT_CALL(mocklogger, logError(HasSubstr("assert_streq: Input string invalid"))).Times(2);

  // Test string equal
  ASSERT_EQ(assert.TA_ASSERT_STREQ("abc", "abc"), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.TA_ASSERT_STREQ("abc", "abd"), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_STREQ("abc", "abce"), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_STREQ("ab", "abc"), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_STREQ("abc", "\0"), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.TA_ASSERT_STREQ("abc", NULL), RetCode::RETCODE_NULL_POINTER);
  ASSERT_EQ(assert.TA_ASSERT_STREQ(NULL, "abc"), RetCode::RETCODE_NULL_POINTER);
}

TEST(Assert_TestSuite, test_assert_streq_logger_error) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;
  assert.setLogger(mocklogger);

  EXPECT_CALL(mocklogger, logError).WillOnce(Return(RetCode::RETCODE_FAILURE));

  // Force call logError in assert_streq. Happens when assert fails
  ASSERT_EQ(assert.TA_ASSERT_STREQ("abc", "abdc"), RetCode::RETCODE_UNEXPECTED_BEHAVIOR);
}

TEST(Assert_TestSuite, returnReportAndReset) {
  Assert& assert = Assert::getInstance();
  MockLogger mocklogger;
  assert.setLogger(mocklogger);

  EXPECT_CALL(mocklogger, logError(HasSubstr("Failure ASSERT_TRUE input: false"))).Times(1);
  EXPECT_CALL(mocklogger, logError(HasSubstr("assert_streq: Input string invalid"))).Times(1);

  // Test with oversized fileName (leaves not enough room for full report)
  {
    char oversizedFilename[250];
    for (uint16_t i = 0; i < 250; i++) {
      oversizedFilename[i] = 'x';
    }
    oversizedFilename[249] = '\0';
    AssertReport AssertReport(AssertTypes::ASSERT_GE, 1.0, 1.0, RetCode::RETCODE_SUCCESS, 5, oversizedFilename);
    ASSERT_EQ(assert.logAssert(AssertReport), RetCode::RETCODE_UNEXPECTED_BEHAVIOR);
    ASSERT_EQ(assert.returnReportAndReset(), ReportResult::ReportFailure);
  }

  // Test with failed test
  ASSERT_EQ(assert.TA_ASSERT_TRUE(false), RetCode::RETCODE_FAILURE);
  ASSERT_EQ(assert.returnReportAndReset(), ReportResult::ReportFailure);

  // Test if report successful after reset
  ASSERT_EQ(assert.returnReportAndReset(), ReportResult::ReportSuccess);

  // Test with nullpointer-failed streq
  ASSERT_EQ(assert.TA_ASSERT_STREQ(NULL, "ab"), RetCode::RETCODE_NULL_POINTER);
  ASSERT_EQ(assert.returnReportAndReset(), ReportResult::ReportFailure);

  // Test with successful test
  ASSERT_EQ(assert.TA_ASSERT_STREQ("ab", "ab"), RetCode::RETCODE_SUCCESS);
  ASSERT_EQ(assert.returnReportAndReset(), ReportResult::ReportSuccess);
}
