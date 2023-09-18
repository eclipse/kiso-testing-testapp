/**
 * @file include/TestRegistry/TestRegistry/TestRegistry.hpp
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

#ifndef TESTREGISTRY_TESTREGISTRY_HPP
#define TESTREGISTRY_TESTREGISTRY_HPP

#include <TestFactory/TestFactoryBase.hpp>
#include <TestSuite/TestSuiteInterface.hpp>
#include <TestCase/TestCaseInterface.hpp>
#include <array>

/**
* @brief Dummy Class to be passed to the TestRegistry as it is a root factory class, 
*        and there is no parent factory class associated with it
*/
class NoFactory{
};

/**
 * @brief TestRegistry Class which is a singleton that holds the registered TestSuites
*/
class TestRegistry final : public TestFactoryBase<NoFactory, testsuite::TestSuiteInterface> {
public:
  /// Deleted copy constructor
  TestRegistry(const TestRegistry &) = delete;
  /// Deleted move constructor
  TestRegistry(const TestRegistry &&) = delete;
  /// Deleted copy assignment
  TestRegistry &operator=(const TestRegistry&) = delete;
  /// Deleted move assignment
  TestRegistry &operator=(TestRegistry &&) = delete;
  
  /// Returns the singleton instance
  static TestRegistry& getInstance() {
    static TestRegistry testRegistry;
    return testRegistry;
  }

  /// Loads a desired TestSuite, it is not completed yet as
  /// we have to reflect the usage of the IDs and also perform error handling
  testsuite::TestSuiteInterface* loadTestSuite(testapp::TestIDType testSuiteID) {
    return TestRegistry::createObject(testSuiteID, static_cast<void*>(_testSuiteSpace.data()), maxTestSuiteSpace);
  }

  /// Loads a desired TestCase, it is not completed yet as
  /// we have to reflect the usage of the IDs and also perform error handling
  testcase::TestCaseInterface* loadTestCase(testapp::TestIDType testSuiteID, testapp::TestIDType testCaseID) {
    if (auto *tsuit = TestRegistry::createObject(testSuiteID, static_cast<void*>(_testSuiteSpace.data()), maxTestSuiteSpace)) {
      if (auto *tcase = tsuit->createTestCase(testCaseID, static_cast<void*>(_testcaseSpace.data()), maxTestCaseSpace)) {
        return tcase;
      }
    }
    return nullptr;
  }

  static constexpr size_t testSuiteMemoryCapacity() {
    return(maxTestSuiteSpace - sizeof(testsuite::TestSuiteInterface));
  }

  static constexpr size_t testCaseMemoryCapacity() {
    return(maxTestCaseSpace - sizeof(testcase::TestCaseInterface));
  }

private:
  /// A static storage place for loading the desired TestSuite and TestCase, using
  /// the placement new operator, the implementation is not finished yet as it should
  /// checks the required spaces by the relevant TestCase or TestSuite, so it is
  /// hard-coded now
  static constexpr size_t maxTestSuiteSpace = 200; 
  static constexpr size_t maxTestCaseSpace = 200;
  std::array<uint8_t, maxTestSuiteSpace> _testSuiteSpace;
  std::array<uint8_t, maxTestCaseSpace> _testcaseSpace;

  /// Hiding the default constructor and destructor to generate compiler errors
  /// in case of abuse of singleton
  TestRegistry() = default;
  ~TestRegistry() = default;
};

#endif
