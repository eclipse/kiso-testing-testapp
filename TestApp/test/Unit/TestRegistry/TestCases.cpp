#include "TestSuites.cpp"
#include <TestCase/TestCaseBase.hpp>

DECLARE_TESTCASE(TestSuite1, Ts1Tc1, 1)
  uint8_t _data[100];

  testapp::ErrCode doSetup() override {
    return testapp::ErrCode::SUCCESS; 
  }
  
  testapp::ErrCode runTest() override {
    return testapp::ErrCode::SUCCESS; 
  }

  testapp::ErrCode doTearDown() override { 
    return testapp::ErrCode::SUCCESS; 
  }
  
END_TESTCASE(TestCase1)

DECLARE_TESTCASE(TestSuite1, Ts1Tc2, 2)
  uint8_t _data[100];

  testapp::ErrCode doSetup() override {
    return testapp::ErrCode::SUCCESS; 
  }
  
  testapp::ErrCode runTest() override {
    return testapp::ErrCode::SUCCESS; 
  }

  testapp::ErrCode doTearDown() override { 
    return testapp::ErrCode::SUCCESS; 
  }
  
END_TESTCASE(TestCase1)

DECLARE_TESTCASE(TestSuite2, Ts2Tc1, 1)
  uint8_t _data[100];

  testapp::ErrCode doSetup() override {
    return testapp::ErrCode::SUCCESS; 
  }
  
  testapp::ErrCode runTest() override {
    return testapp::ErrCode::SUCCESS; 
  }

  testapp::ErrCode doTearDown() override { 
    return testapp::ErrCode::SUCCESS; 
  }
  
END_TESTCASE(TestCase1)

DECLARE_TESTCASE(TestSuite2, Ts2Tc2, 2)
  uint8_t _data[100];

  testapp::ErrCode doSetup() override {
    return testapp::ErrCode::SUCCESS; 
  }
  
  testapp::ErrCode runTest() override {
    return testapp::ErrCode::SUCCESS; 
  }

  testapp::ErrCode doTearDown() override { 
    return testapp::ErrCode::SUCCESS; 
  }
  
END_TESTCASE(TestCase1)
