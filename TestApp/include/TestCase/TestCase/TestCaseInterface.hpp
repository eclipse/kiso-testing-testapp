/**
 * @file TestCaseAbstract.hpp
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

#ifndef TESTCASEINTERFACE_HPP
#define TESTCASEINTERFACE_HPP

#include <cstdint>
#include <string>
#include <TestAppTypes/TestAppTypes.hpp>
namespace testcase {
/**
 * @brief The interface for a test case
 */
class TestCaseInterface {
public:
  /// Constructor.
  TestCaseInterface(void) = default;

  /// Copy constructor.
  TestCaseInterface(TestCaseInterface const&) = delete;

  /// Move constructor.
  TestCaseInterface(TestCaseInterface&&) = delete;

  /// @brief Copy assignment operator.
  /// @return result of copy assignment
  TestCaseInterface& operator=(TestCaseInterface const&) = delete;

  /// @brief Move assignment operator.
  /// @return result of move assignment
  TestCaseInterface& operator=(TestCaseInterface&&) = delete;

  /// Default destructor.
  virtual ~TestCaseInterface(void) = default;

  /**
   * @brief Prepares environment for testcase execution
   * 
   * @return testapp::ErrCode
   */
  virtual testapp::ErrCode doSetup() = 0;
  
  /**
   * @brief Method to execute the different steps of the testcase specification.
   * @param TestStatus reference to the object holding the Status during the execution
   *  
   * Note The method can be called multiple times within the execution context and will execute different steps on the StatusC object state
   * 
   * @return testapp::ErrCode
   */
  virtual testapp::ErrCode runTest() = 0;
  
  /**
   * @brief Method to implement the testcase teardown and reset the Environment for further testcase execution
   */
  virtual testapp::ErrCode doTearDown() = 0;
};

}  // namespace testcase

#endif
