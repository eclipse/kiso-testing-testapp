#include <TestRegistry/TestRegistry.hpp>
#include "TestCases.cpp"

#include <cstring>  // std::memcmp
#include <ostream> // TO be removed

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(TestRegistryTest, TestExistance) {
  TestRegistry &testRegistry = TestRegistry::getInstance();
  {
    auto tsi = testRegistry.loadTestSuite(1);
    ASSERT_TRUE(tsi);
    auto tci = testRegistry.loadTestCase(1, 1);
    ASSERT_TRUE(tci);
    tci = testRegistry.loadTestCase(1, 2);
    ASSERT_TRUE(tci);
  }
  {
	  auto tsi = testRegistry.loadTestSuite(2);
    ASSERT_TRUE(tsi);
    auto tci = testRegistry.loadTestCase(2, 1);
    ASSERT_TRUE(tci);
    tci = testRegistry.loadTestCase(2, 2);
    ASSERT_TRUE(tci);
  }
}

TEST(TestRegistryTest, TestNonExistance) {
  TestRegistry &testRegistry = TestRegistry::getInstance();
  {
    for(int i = 4; i < 50; ++i) {
      auto tsi = testRegistry.loadTestSuite(i);
      ASSERT_FALSE(tsi);
    }
  }
  {
	  auto tsi = testRegistry.loadTestSuite(1);
    ASSERT_TRUE(tsi);
    for(int i = 4; i < 50; ++i) {
      auto tci = testRegistry.loadTestCase(1, i);
      ASSERT_FALSE(tci);
    }
  }
  {
	  auto tsi = testRegistry.loadTestSuite(2);
    ASSERT_TRUE(tsi);
    for(int i = 4; i < 50; ++i) {
      auto tci = testRegistry.loadTestCase(2, i);
      ASSERT_FALSE(tci);
    }
  }
}

TEST(TestRegistryTest, TestSimpleDataMember) {
  TestRegistry &testRegistry = TestRegistry::getInstance();
	{
    auto tsi = testRegistry.loadTestSuite(1);
    ASSERT_TRUE(tsi);
    TestSuite1& ts = *static_cast<TestSuite1*>(tsi);
    for(size_t i = 0; i < sizeof(ts._data); ++i)
      ts._data[i] = i;
  }
  {
	  auto tsi = testRegistry.loadTestSuite(2);
    ASSERT_TRUE(tsi);
    TestSuite2& ts = *static_cast<TestSuite2*>(tsi);
    for(size_t i = 0; i < sizeof(ts._data); ++i)
      EXPECT_EQ(ts._data[i], i);
  }
}

TEST(TestRegistryTest, TestOverCapacity) {
  TestRegistry &testRegistry = TestRegistry::getInstance();
	{
    auto tsi = testRegistry.loadTestSuite(3);
    ASSERT_FALSE(tsi);
  }
}
