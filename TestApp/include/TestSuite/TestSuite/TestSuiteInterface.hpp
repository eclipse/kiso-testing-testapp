/**
 * @file TestSuiteAbstract.hpp
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

#ifndef TESTSUITEABSTRACT_HPP
#define TESTSUITEABSTRACT_HPP

#include <cstdint>
#include <TestCase/TestCaseInterface.hpp>
#include <TestAppTypes/TestAppTypes.hpp>

namespace testsuite {
/**
 * @brief The interface for a test suite
 */
class TestSuiteInterface {
public:
  /// Constructor.
  TestSuiteInterface(void) = default;

  /// Copy constructor.
  TestSuiteInterface(TestSuiteInterface const&) = delete;

  /// Move constructor.
  TestSuiteInterface(TestSuiteInterface&&) = delete;

  /// @brief Copy assignment operator.
  /// @return result of copy assignment
  TestSuiteInterface& operator=(TestSuiteInterface const&) = delete;

  /// @brief Move assignment operator.
  /// @return result of move assignment
  TestSuiteInterface& operator=(TestSuiteInterface&&) = delete;

  /// Default destructor.
  virtual ~TestSuiteInterface(void) = default;

  /**
   * @brief Prepares environment for testcase execution
   * 
   * @return testapp::ErrCode
   */
  virtual testapp::ErrCode doSuiteSetup() = 0;
  
  /**
   * @brief Method to implement the testsuite teardown and reset the environment to load the new TestSuite
   * 
   * @return testapp::ErrCode
   */
  virtual testapp::ErrCode doSuiteTearDown() = 0;

  /**
   * @brief Method to instantiate a possible registered test case in this suit
   * 
   * @return TestCaseInterface*
   */  virtual testcase::TestCaseInterface* createTestCase(testapp::TestIDType testCaseID, void *memoryLocation, size_t memorySize) = 0;

};

}  // namespace testsuite
#endif
