#include <TestSuite/TestSuiteBase.hpp>
#include <TestRegistry/TestRegistry.hpp>

DECLARE_TESTSUITE(TestSuite1, 1)
  testapp::ErrCode doSuiteSetup() override {
    return testapp::ErrCode::SUCCESS;
  }

  testapp::ErrCode doSuiteTearDown() override {
    return testapp::ErrCode::SUCCESS; 
  }
  uint8_t _data[TestRegistry::testSuiteMemoryCapacity()];
END_TESTSUITE(TestSuite1) 

DECLARE_TESTSUITE(TestSuite2, 2)
  testapp::ErrCode doSuiteSetup() override {
    return testapp::ErrCode::SUCCESS;
  }

  testapp::ErrCode doSuiteTearDown() override {
    return testapp::ErrCode::SUCCESS; 
  }
  uint8_t _data[TestRegistry::testSuiteMemoryCapacity()];
END_TESTSUITE(TestSuite2)

DECLARE_TESTSUITE(TestSuite3, 3)
  testapp::ErrCode doSuiteSetup() override {
    return testapp::ErrCode::SUCCESS;
  }

  testapp::ErrCode doSuiteTearDown() override {
    return testapp::ErrCode::SUCCESS; 
  }
  uint8_t _data[TestRegistry::testSuiteMemoryCapacity()+1];
END_TESTSUITE(TestSuite2)
