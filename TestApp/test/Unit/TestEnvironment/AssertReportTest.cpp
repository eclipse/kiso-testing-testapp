/**
 * @file test/Unit/TestEnvironment/AssertReportTest.cpp
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
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <TestEnvironment/AssertReport.hpp>

using namespace testing;
using namespace testapp;
using namespace testenvironment;

// Test class AssertReportResult_T
TEST(AssertReportResult_TestSuite, AssertReport_setFilename) {  // Create Testinstance
  char fileName[AssertReport::maxFilenameSize] = "";
  AssertReport assertReport(AssertTypes::ASSERT_TRUE, 0.0, 0.0, RetCode::RETCODE_SUCCESS, 5,
                            fileName);

  // create testrings with size of AssertReport::maxFilenameSize and
  // AssertReport::maxFilenameSize +1
  char longStringMaxSize[AssertReport::maxFilenameSize] = "";
  char longStringExceedsMaxSize[AssertReport::maxFilenameSize + 1] = "";

  // Fill teststrings with something
  for (uint16_t i = 0; i < AssertReport::maxFilenameSize; i++) {
    longStringMaxSize[i] = 'x';
    longStringExceedsMaxSize[i] = 'x';
  }

  // Set the termination character '\0' at the end of the string
  longStringMaxSize[AssertReport::maxFilenameSize - 1] = '\0';
  longStringExceedsMaxSize[AssertReport::maxFilenameSize] = '\0';

  // Test path with '/'
  ASSERT_EQ(assertReport.setFilename("awesomePath/foo"),
            RetCode::RETCODE_SUCCESS);
  ASSERT_STREQ(assertReport.getFilename(), "foo");

  // Test path with mulitiple '/'
  ASSERT_EQ(assertReport.setFilename("awesomePath/awesomeSubPath/foo"),
            RetCode::RETCODE_SUCCESS);
  ASSERT_STREQ(assertReport.getFilename(), "foo");

  // Test plain fileName
  ASSERT_EQ(assertReport.setFilename("foo"), RetCode::RETCODE_SUCCESS);
  ASSERT_STREQ(assertReport.getFilename(), "foo");

  // Test if string is empty
  ASSERT_EQ(assertReport.setFilename(""), RetCode::RETCODE_SUCCESS);
  ASSERT_STREQ(assertReport.getFilename(), "");

  // Test when path has AssertReport::maxFilenameSize length
  ASSERT_EQ(assertReport.setFilename(longStringMaxSize),
            RetCode::RETCODE_SUCCESS);
  ASSERT_STREQ(assertReport.getFilename(), longStringMaxSize);

  // Test if path exceeds AssertReport::maxFilenameSize length
  ASSERT_EQ(assertReport.setFilename(longStringExceedsMaxSize),
            RetCode::RETCODE_INVALID_PARAM);
  ASSERT_STREQ(assertReport.getFilename(),
               "Filename string not terminated or too long");
}

TEST(AssertReportResult_TestSuite, AssertReport_createReport) {
  char report[AssertReport::maxReportSize] = "";
  {
    AssertReport assertReport(AssertTypes::ASSERT_TRUE, 0.0, 0.0, RetCode::RETCODE_SUCCESS,
                              5, "");

    // Test with empty fileName
    ASSERT_EQ(assertReport.createReport(report), RetCode::RETCODE_SUCCESS);
    ASSERT_STREQ(report, ":5: Success ASSERT_TRUE input: false");
  }

  AssertReport assertReport(AssertTypes::ASSERT_TRUE, 1.0, 1.0, RetCode::RETCODE_SUCCESS, 5,
                            "");
  // Test with reasonably sized fileName
  assertReport.setFilename("testFileName");
  ASSERT_EQ(assertReport.createReport(report), RetCode::RETCODE_SUCCESS);
  ASSERT_STREQ(report, "testFileName:5: Success ASSERT_TRUE input: true");

  // Test with oversized fileName (leaves not enough room for full report)
  char oversizedFilename[250];
  for (uint16_t i = 0; i < 250; i++) {
    oversizedFilename[i] = 'x';
  }
  oversizedFilename[249] = '\0';
  assertReport.setFilename(oversizedFilename);
  ASSERT_EQ(assertReport.createReport(report), RetCode::RETCODE_FAILURE);
  ASSERT_STREQ(report, "Report too long");
}
