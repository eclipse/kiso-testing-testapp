/**
 * @file TestCaseBase.hpp
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 */

#ifndef TESTCASEBASE_HPP
#define TESTCASEBASE_HPP

#include <TestCase/TestCaseInterface.hpp>
#include <TestFactory/TestFactoryRegistrar.hpp>
#include <TestEnvironment/Logging.hpp>
#include <TestEnvironment/Assert.hpp>

/// Capture a reference to logging singleton
static testenvironment::LoggingInterface& logging = testenvironment::Logging::getInstance();

/// Capture a reference to Assert singleton
static testenvironment::Assert& assertion = testenvironment::Assert::getInstance();

/**
 * @brief: TestCaseBase Class, it is a helper class to automatically register any derived TestCase
 * @tparam NewTestCaseClass the new TestCase class
 * @tparam ItsTestSuiteClass the parent TestSuite which is a factory for this class
 */
template<typename ItsTestSuiteClass, typename NewTestCaseClass, testapp::TestIDType ID>
class TestCaseBase : protected TestFactoryRegistrar<testcase::TestCaseInterface, NewTestCaseClass, ItsTestSuiteClass, ID> {  
public:
  TestCaseBase() {
    assertion.setLogger(logging);
  }

  testapp::ErrCode returnReportAndReset() {
    // A successful TestCase or TestSuite shall have no failure, otherwise a failure report is sent
    // Evaluate assertions and logging messages such as errors and critical logs and reset.
    testapp::ErrCode errCodeLogger = logging.getFailureFlag() ? testapp::ErrCode::FAILURE : testapp::ErrCode::SUCCESS;
    logging.resetFailureFlag();

    testapp::ErrCode errCodeAssert = (assertion.returnReportAndReset() == testenvironment::ReportResult::ReportSuccess) ?
      testapp::ErrCode::SUCCESS : testapp::ErrCode::FAILURE;
    
    return (errCodeLogger != testapp::ErrCode::SUCCESS || errCodeAssert != testapp::ErrCode::SUCCESS) ? 
      testapp::ErrCode::FAILURE : testapp::ErrCode::SUCCESS;
  }

};

/// Provides a macro for the start of defining a TestCase
//NOLINTNEXTLINE [cppcoreguidelines-macro-usage] we want to keep googletest style
#define DECLARE_TESTCASE(ITS_TESTSUITE_CLASS, NEW_TESTCASE_CLASS, ID) \
class NEW_TESTCASE_CLASS : public testcase::TestCaseInterface, public TestCaseBase<ITS_TESTSUITE_CLASS, NEW_TESTCASE_CLASS, ID> { /*NOLINT [bugprone-macro-parentheses]*/ \
public: \
    NEW_TESTCASE_CLASS() {(void)registered;} \

/// Provides a macro for the end of defining a TestCase
//NOLINTNEXTLINE [cppcoreguidelines-macro-usage] we want to keep googletest style
#define END_TESTCASE(NEW_TESTCASE_CLASS) };

#endif
