
#include "MessageHandler/MessageHandler.hpp"
#include <CommunicationChannelMock/CommunicationChannelMock.hpp>
#include <MessageBufferReferenceMock/MessageBufferReferenceMock.hpp>
#include <OsExtension/SemaphoreMock.hpp>
#include "TPC/TPC.hpp"

#include <cstring> // std::memcmp
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <etl/array_view.h>

using ::testing::A;
using ::testing::_;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;


std::vector<std::vector<uint8_t>> getValidITFMessages()
{

  std::vector<std::vector<uint8_t>> ValidITFMessages = {
      {0x40, 0x1, 0x0, 0x0, 0x0, 0x55, 0x55, 0x0, 0xc5, 0xa},
      {0x40, 0x1, 0x0, 0x0, 0x0, 0xaa, 0xaa, 0x0, 0x59, 0xc6},
      {0x40, 0x2, 0x2, 0x0, 0x0, 0x1, 0x0, 0x0, 0x43, 0x2a},
      {0x60, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5b, 0xed},
      {0x40, 0x3, 0x3, 0x0, 0x0, 0x0, 0x1, 0x0, 0x83, 0xd3},
      {0x60, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3a, 0x55},
      {0x40, 0x4, 0xd, 0x0, 0x0, 0x0, 0x0, 0x0, 0x55, 0x79},
      {0x60, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7e, 0x4c},
      {0x50, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x6e, 0xc, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2d, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x2d, 0x95},
      {0x60, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0xf4},
      {0x40, 0x6, 0x17, 0x0, 0x0, 0x0, 0x0, 0x0, 0x30, 0x85},
      {0x60, 0x6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x9d, 0x2c},
      {0x40, 0x7, 0x16, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf1, 0x78},
      {0x40, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x94, 0xef}

  };

  return ValidITFMessages;
}

namespace tccinterface
{
   TccInterface *tccInterface = nullptr;
} // namespace tccinterface

 class TPCTest : public testing::Test
{
public:
  osextension::MessageBufferReferenceMock _TestRunnerToTPCMessageBuffer;
  osextension::MessageBufferReferenceMock _TPCToTestRunnerMessageBuffer;
  tpcprotocol::TPC tpc{};
  message::MessageStructure dataReceivedFromITF{};
  tccinterface::TccInterfaceMock tccInterfaceMock{};

  virtual void SetUp()
  {
    tccinterface::tccInterface = &tccInterfaceMock;
    tpc.setTPCToTestRunnerMessageBufferReference(_TPCToTestRunnerMessageBuffer);
    tpc.setTestRunnerTPCMessageBufferReference(_TestRunnerToTPCMessageBuffer);
  }

  virtual void TearDown()
  {
    tccinterface::tccInterface = nullptr;
  }
};

 TEST_F(TPCTest, ValidateInCorrectITFMessageBunch1)
 {
   auto ValidITFMessages = getValidITFMessages();
   for (uint8_t i = 0; i < ValidITFMessages.size(); i++)
   {
     ValidITFMessages[i][ValidITFMessages[i].size() - 1]++;
     
     etl::array_view<uint8_t> ValidITFMessagesView{ValidITFMessages[i].begin(), ValidITFMessages[i].size()};
     auto ret = tpc.validateITFByteStream(ValidITFMessagesView, ValidITFMessagesView.size());
     EXPECT_FALSE(ret);
   }

 }

TEST_F(TPCTest, ValidateCorrectITFMessageTest)
{
  auto ValidITFMessages = getValidITFMessages();

  for (uint8_t i = 0; i < ValidITFMessages.size(); i++)
  {
    etl::array_view<uint8_t> ValidITFMessagesView{ValidITFMessages[i].begin(), ValidITFMessages[i].size()};
    bool ret = tpc.validateITFByteStream(ValidITFMessagesView, ValidITFMessagesView.size());
    EXPECT_TRUE(ret);
  }
}

TEST_F(TPCTest, SendReportMessageToITF)
{
  const uint8_t nbTestCases = 6u;

  //Input Data
  testapp::LogMessageT dataReceivedFromTestRunner[nbTestCases] = {
      {testapp::MessageType::MESSAGE_INFO, "INFO: process test TESTCASE_TEARDOWN"},
      {testapp::MessageType::MESSAGE_DEBUG, "DEBUG: process test TESTSUITE_SETUP"},
      {testapp::MessageType::MESSAGE_WARNING, "WARNING: process test TESTSUITE_TEARDOWN"},
      {testapp::MessageType::MESSAGE_ERROR, "ERROR: process test suite TESTCASE_SETUP"},
      {testapp::MessageType::MESSAGE_CRITICAL, "CRITICAL: process test TESTCASE_RUN"},
      {testapp::MessageType::MESSAGE_LOG, "LOG: process test TESTCASE_TEARDOWN"}};

  message::MessageStructure _ReportMessage[nbTestCases] = {
      {{0, 1, 0}, message::MessageType::Report, 0u, message::MessageSubType::TESTCASE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(dataReceivedFromTestRunner[0].text.data())))) + 2), {}},
      {{1, 1, 0}, message::MessageType::Report, 1u, message::MessageSubType::TESTSUITE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(dataReceivedFromTestRunner[1].text.data())))) + 2), {}},
      {{2, 1, 0}, message::MessageType::Report, 2u, message::MessageSubType::TESTSUITE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(dataReceivedFromTestRunner[2].text.data())))) + 2), {}},
      {{0, 1, 0}, message::MessageType::Report, 3u, message::MessageSubType::TESTCASE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 2u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(dataReceivedFromTestRunner[3].text.data())))) + 2), {}},
      {{1, 1, 0}, message::MessageType::Report, 4u, message::MessageSubType::TESTCASE_RUN, testapp::ErrCode::SUCCESS, 0u, 1u, 2u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(dataReceivedFromTestRunner[4].text.data())))) + 2), {}},
      {{2, 1, 0}, message::MessageType::Report, 5u, message::MessageSubType::TESTCASE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 1u, (uint8_t)(strlen(const_cast<const char*>(static_cast<char*>(static_cast<void*>(dataReceivedFromTestRunner[5].text.data())))) + 2), {}}};

  for (uint8_t iter = 0; iter < nbTestCases; iter++)
  {
    message::MessageStructure reportToTPC = _ReportMessage[iter];
    EXPECT_CALL(tccInterfaceMock, send(_)).Times(1);

    tpc.sendReportToITF(reportToTPC, message::messageHeaderSize + reportToTPC.payloadLength);
  }
}

TEST_F(TPCTest, oneCycleTest_ITF_TPC_recieve)
{
  //Input data
  dataReceivedFromITF = {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTSUITE_SETUP, testapp::ErrCode::SUCCESS, 0u, 2u, 0u, 0u, {}};

  EXPECT_CALL(tccInterfaceMock, receive(_, A<etl::array_view<uint8_t>&>())).Times(1);
  EXPECT_CALL(tccInterfaceMock, send(_)).Times(0);
  EXPECT_CALL(_TPCToTestRunnerMessageBuffer, send(A<const void*>(), _)).Times(0);
  //function call
  tpc.oneCycle();
}
