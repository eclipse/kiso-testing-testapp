/**
 * @file src/TestEnvironment/Assert.cpp
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
#include <TestEnvironment/Assert.hpp>

namespace testenvironment {

Assert& Assert::getInstance(void) {
  static Assert instance;
  return instance;
}

testapp::RetCode Assert::assertStrEq(const char* str1, const char* str2, const int line, const char* file) {
  // Check if Logger has been set
  if (!isLoggerInitialized()) {
    _report = ReportResult::ReportFailure;
    return testapp::RetCode::RETCODE_UNINITIALIZED;
  }

  // Check if input strings are valid
  if (nullptr == str1 || nullptr == str2) {
    _logger->logError("assert_streq: Input string invalid");
    _report = ReportResult::ReportFailure;
    return testapp::RetCode::RETCODE_NULL_POINTER;
  }

  testapp::RetCode retCode = (strcmp(str1, str2) == 0) ? testapp::RetCode::RETCODE_SUCCESS : testapp::RetCode::RETCODE_FAILURE;
  AssertReport assertReport(AssertTypes::ASSERT_STREQ, str1, str2, retCode, line, file);
  if (logAssert(assertReport) != testapp::RetCode::RETCODE_SUCCESS) {
    retCode = testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR;
  }

  if (retCode != testapp::RetCode::RETCODE_SUCCESS) {
    _report = ReportResult::ReportFailure;
  }
  return retCode;
}

testapp::RetCode Assert::logAssert(const AssertReport& assertReport) {
  testapp::RetCode retCode = testapp::RetCode::RETCODE_SUCCESS;
  std::array<char, testapp::maxAssertStringLength> tmpMessage = {'\0'};

  // Check if Logger has been set
  if (!isLoggerInitialized()) {
    _report = ReportResult::ReportFailure;
    return testapp::RetCode::RETCODE_UNINITIALIZED;
  }

  // Create report and store it
  if (assertReport.createReport(tmpMessage.data()) != testapp::RetCode::RETCODE_SUCCESS) {
    _report = ReportResult::ReportFailure;
    return testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR;
  }

  // Call log Module only if test detected a failure
  if (assertReport.getResult() != testapp::RetCode::RETCODE_SUCCESS) {
    retCode = _logger->logError(tmpMessage.data());
  }

  if (retCode != testapp::RetCode::RETCODE_SUCCESS) {
    _report = ReportResult::ReportFailure;
  }

  return retCode;
}

testapp::RetCode Assert::setLogger(LoggingInterface& logger) {
  _logger = &logger;
  return testapp::RetCode::RETCODE_SUCCESS;
}

ReportResult Assert::returnReportAndReset() {
  ReportResult returnCode = _report;
  _report = ReportResult::ReportSuccess;
  return returnCode;
}

bool Assert::isLoggerInitialized(void) const {
  return (nullptr != _logger) ? true : false;
}

}  // namespace testenvironment
