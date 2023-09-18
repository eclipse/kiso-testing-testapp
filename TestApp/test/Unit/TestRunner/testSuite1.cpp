/**
 * @file examples/test_suite/test_suite_1.cpp
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

#include <TestSuite/TestSuiteBase.hpp>
#include <TestCase/TestCaseBase.hpp>
#include <TestEnvironment/Assert.hpp>
#include <TestEnvironment/Logging.hpp>

// Test Suite DummyTestSuite1
DECLARE_TESTSUITE(DummyTestSuite1, 1)
testapp::ErrCode doSuiteSetup() override {

  logging.logInfo("TestSuite 1 - 1st doSuiteSetup Hello TestApp!");
  logging.logDebug("TestSuite 1 - 2nd Log message");
  logging.logWarning("TestSuite 1 - 3rd Log message");
  assertion.TA_ASSERT_TRUE(true);
  // send error and critical log (test sould fail)
  logging.logError("TestSuite 1 - error Log-message");
  logging.logCritical("TestSuite 1 - critical error Log-message");
  logging.logInfo("TestSuite 1 doSuiteSetup should fail");
  
  return returnReportAndReset();
}

testapp::ErrCode doSuiteTearDown() override {

  logging.logInfo("TestSuite 1 doSuiteTearDown");
  assertion.TA_ASSERT_TRUE(true);
  assertion.TA_ASSERT_FALSE(false);
  assertion.TA_ASSERT_EQ(5,5);
  assertion.TA_ASSERT_NE(5,4);

  logging.logInfo("TestSuite 1 doSuiteTearDown should pass successfully");

  return returnReportAndReset();
}
END_TESTSUITE(DummyTestSuite1)

// Tast Suite : DummyTestSuite1
// Test Case  : TestCase1
DECLARE_TESTCASE(DummyTestSuite1, TestCase1, 1)

testapp::ErrCode doSetup() override {

  logging.logInfo("TestCase 1 doSetup");
  assertion.TA_ASSERT_LT(5,6);
  assertion.TA_ASSERT_LE(6,6);
  assertion.TA_ASSERT_STREQ("message1", "message1");
  logging.logInfo("TestCase 1 doSetup should pass successfully");
  
  return returnReportAndReset();
}

testapp::ErrCode runTest() override {

  logging.logInfo("TestCase 1 runTest");
  assertion.TA_ASSERT_GT(7,6);
  assertion.TA_ASSERT_GE(7,7);
  logging.logInfo("TestCase 1 runTest should pass successfully");

  return returnReportAndReset();
}

testapp::ErrCode doTearDown() override {
  logging.logInfo("TestCase 1 doTearDown");

  assertion.TA_ASSERT_STREQ("message1", "message2");
  logging.logInfo("TestCase 1 doTearDown should fail");

  return returnReportAndReset();
}

END_TESTCASE(TestCase1)

// Test Suite : DummyTestSuite1
// Test Case  : TestCase2
DECLARE_TESTCASE(DummyTestSuite1, TestCase2, 2)

testapp::ErrCode doSetup() override {
  logging.logInfo("TestCase 2 Starting Mars");
  return returnReportAndReset();
}

testapp::ErrCode runTest() override {
  logging.logInfo("TestCase 2 Hello Mars");
  return returnReportAndReset();
}

testapp::ErrCode doTearDown() override {
  logging.logInfo("TestCase 2 Ending Mars");
  return returnReportAndReset();
}

END_TESTCASE(TestCase2)
