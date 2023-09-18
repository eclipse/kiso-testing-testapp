/**
 * @file src/TestEnvironment/AssertReport.cpp
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
#include <cstring>
#include <utility>

#include <TestEnvironment/AssertReport.hpp>

namespace testenvironment {

const char* AssertReport::getFilename(void) const {
  return _fileName.data();
}

testapp::RetCode AssertReport::getResult(void) const {
  return _retCode;
}

AssertReport::AssertReport(const AssertTypes assertType, const float input1,
                           const float input2, const testapp::RetCode retCode,
                           const uint16_t line, const char* fileName)
    : _assertType(assertType),
      _input1(input1),
      _input2(input2),
      _inputString1(nullptr),
      _inputString2(nullptr),
      _retCode(retCode),
      _line(line),
      _fileName{} {
  setFilename(fileName);
}

AssertReport::AssertReport(const AssertTypes assertType, const char* input1,
                           const char* input2, const testapp::RetCode retCode,
                           const uint16_t line, const char* fileName)
    : _assertType(assertType),
      _input1(0),
      _input2(0),
      _inputString1(input1),
      _inputString2(input2),
      _retCode(retCode),
      _line(line),
      _fileName{} {
  setFilename(fileName);
}

testapp::RetCode AssertReport::createReport(char* report) const {
  std::array<char, operatorStringMaxSize> operatorString = {'\0'};
  std::array<char, maxReportSize> tmpReport = {'\0'};

  switch (_assertType) {
    case (AssertTypes::ASSERT_TRUE):
      strncpy(operatorString.data(), "ASSERT_TRUE", operatorString.max_size());
      break;
    case (AssertTypes::ASSERT_FALSE):
      strncpy(operatorString.data(), "ASSERT_FALSE", operatorString.max_size());
      break;
    case (AssertTypes::ASSERT_EQ):
      strncpy(operatorString.data(), "ASSERT_EQ", operatorString.max_size());
      break;
    case (AssertTypes::ASSERT_NE):
      strncpy(operatorString.data(), "ASSERT_NE", operatorString.max_size());
      break;
    case (AssertTypes::ASSERT_LT):
      strncpy(operatorString.data(), "ASSERT_LT", operatorString.max_size());
      break;
    case (AssertTypes::ASSERT_LE):
      strncpy(operatorString.data(), "ASSERT_LE", operatorString.max_size());
      break;
    case (AssertTypes::ASSERT_GT):
      strncpy(operatorString.data(), "ASSERT_GT", operatorString.max_size());
      break;
    case (AssertTypes::ASSERT_GE):
      strncpy(operatorString.data(), "ASSERT_GE", operatorString.max_size());
      break;
    case (AssertTypes::ASSERT_STREQ):
      strncpy(operatorString.data(), "ASSERT_STREQ", operatorString.max_size());
      break;
    default:
      return testapp::RetCode::RETCODE_NOT_SUPPORTED;
  }

  int16_t snprintfErrorCode = 0;
  if (AssertTypes::ASSERT_TRUE == _assertType || AssertTypes::ASSERT_FALSE == _assertType) {
    snprintfErrorCode = snprintf( //NOLINT(hicpp-vararg, cppcoreguidelines-pro-type-vararg) The alternative bytestream uses dynamic memory allocation (heap allocation)
        tmpReport.data(), maxReportSize, "%s:%d: %s %s input: %s", _fileName.data(), _line,
        (testapp::RetCode::RETCODE_SUCCESS == _retCode) ? "Success" : "Failure",
        operatorString.data(), (_input1 != 0.0f) ? "true" : "false");
  } else if (AssertTypes::ASSERT_STREQ == _assertType) {
    snprintfErrorCode =
        snprintf(tmpReport.data(), maxReportSize, //NOLINT(hicpp-vararg, cppcoreguidelines-pro-type-vararg) The alternative bytestream uses dynamic memory allocation (heap allocation)
                 "%s:%d: %s %s input 1: %s input 2: %s", _fileName.data(), _line,
                 (testapp::RetCode::RETCODE_SUCCESS == _retCode) ? "Success" : "Failure",
                 operatorString.data(), _inputString1, _inputString2);
  } else {
    auto floatToInt = [](const float x) -> std::pair<int32_t, int32_t> {
      int32_t intpart = static_cast<int32_t>(x);
      int32_t decpart = static_cast<int32_t>((x - intpart) * 100);
      return std::make_pair(intpart, decpart);
    };
    auto i1 = floatToInt(static_cast<float>(_input1));
    auto i2 = floatToInt(static_cast<float>(_input2));
    snprintfErrorCode =
        snprintf(tmpReport.data(), maxReportSize, //NOLINT(hicpp-vararg, cppcoreguidelines-pro-type-vararg) The alternative bytestream uses dynamic memory allocation (heap allocation)
                 "%s:%d: %s %s input 1: %d.%02d input 2: %d.%02d", _fileName.data(), _line,
                 (testapp::RetCode::RETCODE_SUCCESS == _retCode) ? "Success" : "Failure",
                 operatorString.data(), i1.first,i1.second,
                 i2.first,i2.second);
  }

  if ((snprintfErrorCode < 0) || (snprintfErrorCode >= maxReportSize)) {
    strncpy(report, "Report too long", maxReportSize);
    return testapp::RetCode::RETCODE_FAILURE;
  }

  strncpy(report, tmpReport.data(), maxReportSize);
  return testapp::RetCode::RETCODE_SUCCESS;
}

testapp::RetCode AssertReport::setFilename(const char* fileName) {
  uint16_t slashPos = 0;
  uint16_t lenFilename = 0;

  for (uint16_t i = 0U; i < maxFilenameSize; i++) {
    if ('/' == fileName[i]) { //NOLINT (cppcoreguidelines-pro-bounds-pointer-arithmetic): pointer arithmetic can only be resolved with another container
      // Store position of '/'
      slashPos = i;
    } else if ('\0' == fileName[i]) { //NOLINT (cppcoreguidelines-pro-bounds-pointer-arithmetic): pointer arithmetic can only be resolved with another container
      // End Loop when '\0' occurs. lenFilename + 1 because i is 0 indexed
      lenFilename = i + 1;
      break;
    } else {
      // Do nothing
    }
  }

  if (lenFilename > maxFilenameSize || lenFilename == 0) {
    strncpy(_fileName.data(), "Filename string not terminated or too long", maxFilenameSize);
    return testapp::RetCode::RETCODE_INVALID_PARAM;
  }

  // Crop fileName after slash
  if (slashPos > 0) {
    //  If string has a '/'. Copy from '/' to end.
    memcpy(_fileName.data(), &fileName[slashPos + 1], lenFilename - (slashPos)); //NOLINT (cppcoreguidelines-pro-bounds-pointer-arithmetic): pointer arithmetic can only be resolved with another container
  } else {  //  Copy whole string if no '/' was found.
    memcpy(_fileName.data(), fileName, lenFilename);
  }
  return testapp::RetCode::RETCODE_SUCCESS;
}

}  // namespace testenvironment
