/**
 * @file include/TestEnvironment/TestEnvironment/Assert.hpp
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
#ifndef TESTENVIRONMENT_ASSERT_HPP
#define TESTENVIRONMENT_ASSERT_HPP
#include <TestAppTypes/TestAppTypes.hpp>
#include <TestEnvironment/AssertReport.hpp>
#include <TestEnvironment/Logging.hpp>
#include <TestEnvironment/TestEnvironmentTypes.hpp>
namespace testenvironment {

/**
 * Define Test App assertions.
 * Named to TA_ASSERT* because it would conflict with the GTest macros
 */
//NOLINTNEXTLINE [cppcoreguidelines-macro-usage] with source_location from c++20 this __LINE__ and __FILE__ can be replaced in c++ way
#define TA_ASSERT_TRUE(obj) assertTrue(obj, __LINE__, __FILE__)
//NOLINTNEXTLINE [cppcoreguidelines-macro-usage] with source_location from c++20 this __LINE__ and __FILE__ can be replaced in c++ way
#define TA_ASSERT_FALSE(obj) assertFalse(obj, __LINE__, __FILE__)
//NOLINTNEXTLINE [cppcoreguidelines-macro-usage] with source_location from c++20 this __LINE__ and __FILE__ can be replaced in c++ way
#define TA_ASSERT_EQ(objA, objB) assertEq(objA, objB, __LINE__, __FILE__)
//NOLINTNEXTLINE [cppcoreguidelines-macro-usage] with source_location from c++20 this __LINE__ and __FILE__ can be replaced in c++ way
#define TA_ASSERT_NE(objA, objB) assertNe(objA, objB, __LINE__, __FILE__)
//NOLINTNEXTLINE [cppcoreguidelines-macro-usage] with source_location from c++20 this __LINE__ and __FILE__ can be replaced in c++ way
#define TA_ASSERT_LT(objA, objB) assertLt(objA, objB, __LINE__, __FILE__)
//NOLINTNEXTLINE [cppcoreguidelines-macro-usage] with source_location from c++20 this __LINE__ and __FILE__ can be replaced in c++ way
#define TA_ASSERT_LE(objA, objB) assertLe(objA, objB, __LINE__, __FILE__)
//NOLINTNEXTLINE [cppcoreguidelines-macro-usage] with source_location from c++20 this __LINE__ and __FILE__ can be replaced in c++ way
#define TA_ASSERT_STREQ(str1, str2) assertStrEq(str1, str2, __LINE__, __FILE__)
//NOLINTNEXTLINE [cppcoreguidelines-macro-usage] with source_location from c++20 this __LINE__ and __FILE__ can be replaced in c++ way
#define TA_ASSERT_GT(objA, objB) assertGt(objA, objB, __LINE__, __FILE__)
//NOLINTNEXTLINE [cppcoreguidelines-macro-usage] with source_location from c++20 this __LINE__ and __FILE__ can be replaced in c++ way
#define TA_ASSERT_GE(objA, objB) assertGe(objA, objB, __LINE__, __FILE__)

/**
 * @brief Assert class for the execution of assert operations
 *
 * - Provides assert operations
 * - Loggs errors
 * - Provides report of multiple assert operations together (Success/Failure)
 *
 */
class Assert {
public:
  /// Copy constructor.
  Assert(Assert const&) = delete;  // Explicitly deleted

  /// Move constructor.
  Assert(Assert&&) = delete;  // Explicitly deleted

  /// Copy assignment operator.
  Assert& operator=(Assert const&) = delete;  // Explicitly deleted

  /// Move assignment operator.
  Assert& operator=(Assert&&) = delete;  // Explicitly deleted

  /**
   * @brief Get the Instance object
   *
   * @return Assert& Instance object
   */
  static Assert& getInstance(void);

  /**
   * @brief Checks if obj == true
   *
   * @tparam T obj type
   * @param obj obj to be verified
   * @param line line from where the operation has been called from
   * @param file file from where the operation has been called from
   * @return testapp::RetCode::RETCODE_SUCCESS: obj == true
   * @return testapp::RetCode::RETCODE_FAILURE: obj != true
   * @return testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR: Internal error during
   * operation
   */
  template <class T>
  testapp::RetCode assertTrue(const T obj, const int line, const char* file) {
    testapp::RetCode retCode = (obj) ? testapp::RetCode::RETCODE_SUCCESS : testapp::RetCode::RETCODE_FAILURE;
    AssertReport assertReport(AssertTypes::ASSERT_TRUE, static_cast<float>(obj), 1.0F, retCode, line, file);
    if (logAssert(assertReport) != testapp::RetCode::RETCODE_SUCCESS) {
      retCode = testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR;
    }

    if (retCode != testapp::RetCode::RETCODE_SUCCESS) {
      _report = ReportResult::ReportFailure;
    }
    return retCode;
  }

  /**
   * @brief Checks if obj == false
   *
   * @tparam T obj type
   * @param obj obj to be verified
   * @param line line from where the operation has been called from
   * @param file file from where the operation has been called from
   * @return testapp::RetCode::RETCODE_SUCCESS: obj == false
   * @return testapp::RetCode::RETCODE_FAILURE: obj != false
   * @return testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR: Internal error during
   * operation
   */
  template <class T>
  testapp::RetCode assertFalse(const T obj, const int line, const char* file) {
    testapp::RetCode retCode = (!obj) ? testapp::RetCode::RETCODE_SUCCESS : testapp::RetCode::RETCODE_FAILURE;
    AssertReport assertReport(AssertTypes::ASSERT_FALSE, static_cast<float>(obj), 0.0F, retCode, line, file);
    if (logAssert(assertReport) != testapp::RetCode::RETCODE_SUCCESS) {
      retCode = testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR;
    }

    if (retCode != testapp::RetCode::RETCODE_SUCCESS) {
      _report = ReportResult::ReportFailure;
    }
    return retCode;
  }

  /**
   * @brief Checks if objA == objB
   *
   * @tparam T obj type
   * @param objA First object to compare
   * @param objB Second object to compare
   * @param line line from where the operation has been called from
   * @param file file from where the operation has been called from
   * @return testapp::RetCode::RETCODE_SUCCESS: objA == objB
   * @return testapp::RetCode::RETCODE_FAILURE: objA != objB
   * @return testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR: Internal error during
   * operation
   */
  template <class T>
  testapp::RetCode assertEq(const T objA, const T objB, const int line, const char* file) {
    testapp::RetCode retCode = (objA == objB) ? testapp::RetCode::RETCODE_SUCCESS : testapp::RetCode::RETCODE_FAILURE;

    AssertReport assertReport(AssertTypes::ASSERT_EQ, static_cast<float>(objA), static_cast<float>(objB), retCode, line, file);
    if (logAssert(assertReport) != testapp::RetCode::RETCODE_SUCCESS) {
      retCode = testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR;
    }

    if (retCode != testapp::RetCode::RETCODE_SUCCESS) {
      _report = ReportResult::ReportFailure;
    }
    return retCode;
  }

  /**
   * @brief Checks if objA != objB
   *
   * @tparam T obj type
   * @param objA First object to compare
   * @param objB Second object to compare
   * @param line line from where the operation has been called from
   * @param file file from where the operation has been called from
   * @return testapp::RetCode::RETCODE_SUCCESS: objA != objB
   * @return testapp::RetCode::RETCODE_FAILURE: objA == objB
   * @return testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR: Internal error during
   * operation
   */
  template <class T>
  testapp::RetCode assertNe(const T objA, const T objB, const int line, const char* file) {
    testapp::RetCode retCode = (objA != objB) ? testapp::RetCode::RETCODE_SUCCESS : testapp::RetCode::RETCODE_FAILURE;

    AssertReport assertReport(AssertTypes::ASSERT_NE, static_cast<float>(objA), static_cast<float>(objB), retCode, line, file);
    if (logAssert(assertReport) != testapp::RetCode::RETCODE_SUCCESS) {
      retCode = testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR;
    }

    if (retCode != testapp::RetCode::RETCODE_SUCCESS) {
      _report = ReportResult::ReportFailure;
    }
    return retCode;
  }

  /**
   * @brief Checks if objA <  objB
   *
   * @tparam T obj type
   * @param objA First object to compare
   * @param objB Second object to compare
   * @param line line from where the operation has been called from
   * @param file file from where the operation has been called from
   * @return testapp::RetCode::RETCODE_SUCCESS: objA <  objB
   * @return testapp::RetCode::RETCODE_FAILURE: objA >= objB
   * @return testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR: Internal error during
   * operation
   */
  template <class T>
  testapp::RetCode assertLt(const T objA, const T objB, const int line, const char* file) {
    testapp::RetCode retCode = (objA < objB) ? testapp::RetCode::RETCODE_SUCCESS : testapp::RetCode::RETCODE_FAILURE;

    AssertReport assertReport(AssertTypes::ASSERT_LT, static_cast<float>(objA), static_cast<float>(objB), retCode, line, file);
    if (logAssert(assertReport) != testapp::RetCode::RETCODE_SUCCESS) {
      retCode = testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR;
    }

    if (retCode != testapp::RetCode::RETCODE_SUCCESS) {
      _report = ReportResult::ReportFailure;
    }
    return retCode;
  }

  /**
   * @brief Checks if objA <= objB
   *
   * @tparam T obj type
   * @param objA First object to compare
   * @param objB Second object to compare
   * @param line line from where the operation has been called from
   * @param file file from where the operation has been called from
   * @return testapp::RetCode::RETCODE_SUCCESS: objA <= objB
   * @return testapp::RetCode::RETCODE_FAILURE: objA >  objB
   * @return testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR: Internal error during
   * operation
   */
  template <class T>
  testapp::RetCode assertLe(const T objA, const T objB, const int line, const char* file) {
    testapp::RetCode retCode = (objA <= objB) ? testapp::RetCode::RETCODE_SUCCESS : testapp::RetCode::RETCODE_FAILURE;

    AssertReport assertReport(AssertTypes::ASSERT_LE, static_cast<float>(objA), static_cast<float>(objB), retCode, line, file);
    if (logAssert(assertReport) != testapp::RetCode::RETCODE_SUCCESS) {
      retCode = testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR;
    }

    if (retCode != testapp::RetCode::RETCODE_SUCCESS) {
      _report = ReportResult::ReportFailure;
    }
    return retCode;
  }

  /**
   * @brief Checks if objA >  objB
   *
   * @tparam T obj type
   * @param objA First object to compare
   * @param objB Second object to compare
   * @param line line from where the operation has been called from
   * @param file file from where the operation has been called from
   * @return testapp::RetCode::RETCODE_SUCCESS: objA >  objB
   * @return testapp::RetCode::RETCODE_FAILURE: objA <= objB
   * @return testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR: Internal error during
   * operation
   */
  template <class T>
  testapp::RetCode assertGt(const T objA, const T objB, const int line, const char* file) {
    testapp::RetCode retCode = (objA > objB) ? testapp::RetCode::RETCODE_SUCCESS : testapp::RetCode::RETCODE_FAILURE;

    AssertReport assertReport(AssertTypes::ASSERT_GT, static_cast<float>(objA), static_cast<float>(objB), retCode, line, file);
    if (logAssert(assertReport) != testapp::RetCode::RETCODE_SUCCESS) {
      retCode = testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR;
    }

    if (retCode != testapp::RetCode::RETCODE_SUCCESS) {
      _report = ReportResult::ReportFailure;
    }
    return retCode;
  }

  /**
   * @brief Checks if objA >= objB
   *
   * @tparam T obj type
   * @param objA First object to compare
   * @param objB Second object to compare
   * @param line line from where the operation has been called from
   * @param file file from where the operation has been called from
   * @return testapp::RetCode::RETCODE_SUCCESS: objA >= objB
   * @return testapp::RetCode::RETCODE_FAILURE: objA <  objB
   * @return testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR: Internal error during
   * operation
   */
  template <class T>
  testapp::RetCode assertGe(const T objA, const T objB, const int line, const char* file) {
    testapp::RetCode retCode = (objA >= objB) ? testapp::RetCode::RETCODE_SUCCESS : testapp::RetCode::RETCODE_FAILURE;

    AssertReport assertReport(AssertTypes::ASSERT_GE, static_cast<float>(objA), static_cast<float>(objB), retCode, line, file);
    if (logAssert(assertReport) != testapp::RetCode::RETCODE_SUCCESS) {
      retCode = testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR;
    }

    if (retCode != testapp::RetCode::RETCODE_SUCCESS) {
      _report = ReportResult::ReportFailure;
    }
    return retCode;
  }

  /**
   * @brief Checks if str1 == str2
   *
   * @param str1 First string to compare
   * @param str2 Second string to compare
   * @param line line from where the operation has been called from
   * @param file file from where the operation has been called from
   * @return testapp::RetCode::RETCODE_SUCCESS: str1 == str2
   * @return testapp::RetCode::RETCODE_FAILURE: str1 != str2
   * @return testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR: Internal error during
   * operation
   */
  //NOLINTNEXTLINE(readability-avoid-const-params-in-decls) although const here has no effect but to keep declaration and definition similar, it should be ignored
  testapp::RetCode assertStrEq(const char* str1, const char* str2, const int line, const char* file);

  /**
   * @brief Use Logging class to log the assertion if it failed
   *
   * @param AssertReport Assert class with test information
   * @return testapp::RetCode::RETCODE_SUCCESS
   * @return testapp::RetCode::RETCODE_UNINITIALIZED: internal _logger variable has not
   * been set
   * @return testapp::RetCode::RETCODE_UNEXPECTED_BEHAVIOR: Internal error during report
   * creation
   */
  testapp::RetCode logAssert(const AssertReport& assertReport);

  /**
   * @brief Set the Logger object
   *
   * @param logger Logger reference to be set
   * @return testapp::RetCode::RETCODE_SUCCESS
   */
  testapp::RetCode setLogger(LoggingInterface& logger);

  /**
   * @brief Clears the assert instance and returns the report state from the
   * testrun
   *
   * @return ReportResult State of the testrun (Success/Failed)
   */
  ReportResult returnReportAndReset();

private:
  /// Constructor.
  Assert() = default;  // Default implementation
  /// Destructor.
  ~Assert() = default;  // Explicitly defaulted

  /**
   * @brief Queries if the internal logger variable has been set
   *
   * @return true Logger variable has been set
   * @return false Logger variable is still set to nullptr
   */
  bool isLoggerInitialized(void) const;

  LoggingInterface* _logger = nullptr;
  ReportResult _report = ReportResult::ReportSuccess;
};

}  // namespace testenvironment
#endif  // TESTENVIRONMENT_ASSERT_HPP
