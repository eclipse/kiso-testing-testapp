/**
 * @file include/TestEnvironment/TestEnvironment/AssertReport.hpp
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
#ifndef TESTENVIRONMENT_ASSERTREPORT_HPP
#define TESTENVIRONMENT_ASSERTREPORT_HPP

#include <TestAppTypes/TestAppTypes.hpp>
#include <TestEnvironment/TestEnvironmentTypes.hpp>

#include <array>

namespace testenvironment {

  constexpr uint8_t operatorStringMaxSize = 13u;


/**
 * @brief Holds the overall test status for the report
 *
 */
enum class ReportResult { ReportSuccess,
                            ReportFailure };

/**
 * @brief Class for creating a log string for the respective assert operation
 *
 */
class AssertReport {
public:
  /**
   * @brief Returns the stored fileName
   *
   * @return const char* fileName
   */
  const char* getFilename() const;

  /**
   * @brief Returns the stored result of the assert operation
   *
   * @return testapp::RetCode_T Assert operation result
   */
  testapp::RetCode getResult() const;

  /**
   * @brief Construct a new Assert Report object for every assert operation
   * beside string compare
   *
   * @param assertType Type of assert operation
   * @param input1 First object of assert operation
   * @param input2 Second object of assert operation
   * @param retCode Outcome of the assert operation
   * @param line Line number, where the assert operation is written in the file
   * @param fileName File, where the assert operation is written in
   */
  //NOLINTNEXTLINE(readability-avoid-const-params-in-decls) although const here has no effect but to keep declaration and definition similar, it should be ignored
  AssertReport(const AssertTypes assertType, const float input1, const float input2, const testapp::RetCode retCode, const uint16_t line, const char* fileName);

  /**
   * @brief Construct a new Assert Report object for string compare assert
   * operation
   *
   * @param assertType Type of assert operation
   * @param input1 First string of assert operation
   * @param input2 Second string of assert operation
   * @param retCode Outcome of the assert operation
   * @param line Line number, where the assert operation is written in the file
   * @param fileName File, where the assert operation is written in
   */
  //NOLINTNEXTLINE(readability-avoid-const-params-in-decls) although const here has no effect but to keep declaration and definition similar, it should be ignored
  AssertReport(const AssertTypes assertType, const char* input1, const char* input2, const testapp::RetCode retCode, const uint16_t line, const char* fileName);

  /**
   * @brief Create and return a report string for the stored assert operation
   *
   * @param report Pointer to char [UINT8_MAX] array for report
   * @return testapp::RetCode_T Returncode of operation
   */
  testapp::RetCode createReport(char* report) const;

  /**
   * @brief Extract fileName from path string
   *
   * @param fileName Pointer to path string
   * @return testapp::RetCode_T
   */
  testapp::RetCode setFilename(const char* fileName); //NOLINT(readability-avoid-const-params-in-decls) although const here has no effect but to keep declaration and definition similar, it should be ignored
  static constexpr uint8_t maxFilenameSize = UINT8_MAX;
  static constexpr uint8_t maxReportSize = UINT8_MAX;

private:
  const AssertTypes _assertType;
  const float _input1;
  const float _input2;
  const char* _inputString1;
  const char* _inputString2;
  const testapp::RetCode _retCode;
  const uint16_t _line;
  std::array<char, maxFilenameSize> _fileName;
};

}  // namespace testenvironment
#endif  // TESTENVIRONMENT_ASSERTREPORT_HPP
