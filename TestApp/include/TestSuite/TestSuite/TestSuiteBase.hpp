/**
 * @file TestSuiteBase.hpp
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

#ifndef TESTSUITEBASE_HPP
#define TESTSUITEBASE_HPP

#include <TestSuite/TestSuiteInterface.hpp>
#include <TestRegistry/TestRegistry.hpp>
#include <TestCase/TestCaseBase.hpp>
#include <TestCase/TestCaseInterface.hpp>
#include <TestFactory/TestFactoryRegistrar.hpp>

/**
 * @brief: TestSuiteBase Class, it is a helper class to automatically register any derived TestSuite in 
 * the TestRegistry
 * @tparam NewTestSuiteClass the new TestSuite class
 */template<typename NewTestSuiteClass, testapp::TestIDType ID>
class TestSuiteBase : public TestFactoryBase<NewTestSuiteClass, testcase::TestCaseInterface>, 
                      protected TestFactoryRegistrar<testsuite::TestSuiteInterface, NewTestSuiteClass, TestRegistry, ID> { 
public:
  TestSuiteBase() {
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

/// Provides a macro for the start of defining a TestSuite
//NOLINTNEXTLINE [cppcoreguidelines-macro-usage] we want to keep googletest style
#define DECLARE_TESTSUITE(NEW_TESTSUITE_CLASS, ID) \
class NEW_TESTSUITE_CLASS : public testsuite::TestSuiteInterface, public TestSuiteBase<NEW_TESTSUITE_CLASS, ID> { /*NOLINT [bugprone-macro-parentheses]*/ \
public: \
  NEW_TESTSUITE_CLASS() {(void)registered;} \
  testcase::TestCaseInterface* createTestCase(testapp::TestIDType testCaseID, void *memoryLocation, size_t memorySize) final { \
    return(TestSuiteBase::createObject(testCaseID, memoryLocation, memorySize));\
  }
    
/// Provides a macro for the end of defining a TestSuite
//NOLINTNEXTLINE [cppcoreguidelines-macro-usage] we want to keep googletest style
#define END_TESTSUITE(NEW_TESTSUITE_CLASS) };

#endif
