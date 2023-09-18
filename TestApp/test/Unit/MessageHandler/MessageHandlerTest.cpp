/**
 * @file test/MessageHandler/MessageHandlerTest.cpp
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
#include <MessageHandler/MessageHandler.hpp>
#include <gtest/gtest.h>
#include <string>
#include <array>
#include <etl/array_view.h>

void CheckTlv(message::TLV tlv1, message::TLV tlv2)
{
  EXPECT_EQ(tlv1.tag, tlv2.tag);
  EXPECT_EQ(tlv1.length, tlv2.length);

  /*looping through tlv val*/
  for (int i = 0; i < tlv1.length; i++)
  {
    EXPECT_EQ(tlv1.value[i], tlv2.value[i]);
  }
}
TEST(MessageTest, MessageHandlerTest)
{
  message::MessageStructure msg;
  ASSERT_EQ(sizeof(msg.headerInfo), sizeof(uint8_t));

  msg.headerInfo.protocolVersion = 1;
  msg.headerInfo.res = 0;
  msg.token = 0;
  msg.messageType = message::MessageType::Command;
  msg.subType = message::MessageSubType::TESTCASE_SETUP;
  msg.errorCode = testapp::ErrCode::ERRCODE_TEST_CASE_ALREADY_REGISTERED;
  msg.testSection = 1;
  msg.testCase = 2;
  msg.testSuite = 3;
  msg.payloadLength = 0;

  /*adding tlv's*/
  uint8_t arr[11] = {1, 3, 5, 6, 7, 8, 2, 4, 5, 7, 9};

  message::TLV tlvElement;
  tlvElement.tag = 1;
  tlvElement.length = 11;
  tlvElement.value = arr;
  /*call the AddTlv method*/
  auto ret = message::addTLV(tlvElement, msg);
  EXPECT_EQ(ret, testapp::RetCode::RETCODE_SUCCESS);
  /*searialize back to bytearray
  Decode bytearray
  check if all the expected results
*/
  std::array<uint8_t, 255> bytestream;
  etl::array_view<uint8_t> byteStreamView(bytestream);

  message::MessageStructure msg2;
  ret = message::serialize(msg, byteStreamView);

  EXPECT_EQ(ret, testapp::RetCode::RETCODE_SUCCESS);

  ret = message::deserialize(byteStreamView, msg2);
  EXPECT_EQ(ret, testapp::RetCode::RETCODE_SUCCESS);
  EXPECT_EQ(msg.headerInfo.protocolVersion, msg2.headerInfo.protocolVersion);
  EXPECT_EQ(msg.headerInfo.messageType, msg2.headerInfo.messageType);
  EXPECT_EQ(msg.headerInfo.res, msg2.headerInfo.res);
  EXPECT_EQ(msg.token, msg2.token);
  EXPECT_EQ(msg.testSection, msg2.testSection);
  EXPECT_EQ(msg.testSuite, msg2.testSuite);
  EXPECT_EQ(msg.testCase, msg2.testCase);

  message::TLV tlvOpt1;
  message::parseTLV(msg2, tlvOpt1);
  CheckTlv(tlvOpt1, tlvElement);
}

TEST(MessageTest, MessageHandlerMultipleTLVTest)
{
  message::MessageStructure msg;
  msg.headerInfo.protocolVersion = 1;
  msg.headerInfo.res = 0;
  msg.token = 0;
  msg.messageType = message::MessageType::Command;
  msg.subType = message::MessageSubType::TESTCASE_SETUP;
  msg.errorCode = testapp::ErrCode::ERRCODE_TEST_CASE_ALREADY_REGISTERED;
  msg.testSection = 1;
  msg.testCase = 2;
  msg.testSuite = 3;
  msg.payloadLength = 0;

  /*adding multiple tlv's*/
  uint8_t arr[15] = {1, 3, 5, 6, 7, 8, 2, 4, 5, 7, 8, 9, 16, 15, 14};

  message::TLV tlvElement;
  tlvElement.tag = 1;
  tlvElement.length = 15;
  tlvElement.value = arr;
  auto ret = message::addTLV(tlvElement, msg);
  EXPECT_EQ(ret, testapp::RetCode::RETCODE_SUCCESS);

  uint8_t arr1[12] = {1, 3, 5, 6, 7, 8, 2, 4, 5, 7, 8, 9};

  message::TLV tlvElement2;
  tlvElement2.tag = 1;
  tlvElement2.length = 12;
  tlvElement2.value = arr1;
  ret = message::addTLV(tlvElement2, msg);
  EXPECT_EQ(ret, testapp::RetCode::RETCODE_SUCCESS);

  uint8_t arr2[10] = {1, 3, 5, 6, 7, 8, 2, 4, 5, 7};

  message::TLV tlvElement3;
  tlvElement3.tag = 1;
  tlvElement3.length = 10;
  tlvElement3.value = arr2;
  ret = message::addTLV(tlvElement3, msg);
  EXPECT_EQ(ret, testapp::RetCode::RETCODE_SUCCESS);

  std::array<uint8_t, 255> bytestream;
  etl::array_view<uint8_t> byteStreamView(bytestream);

  message::MessageStructure msg2;
  ret = message::serialize(msg, byteStreamView);

  ret = message::deserialize(byteStreamView, msg2);

  message::TLV tlv;
  ret = message::parseTLV(msg2, tlv);
  EXPECT_EQ(ret, testapp::RetCode::RETCODE_SUCCESS);
  CheckTlv(tlv, tlvElement);
  //check tlv with tlvElement
  ret = message::parseTLV(msg2, tlv);
  EXPECT_EQ(ret, testapp::RetCode::RETCODE_SUCCESS);
  CheckTlv(tlv, tlvElement2);
  ret = message::parseTLV(msg2, tlv);
  EXPECT_EQ(ret, testapp::RetCode::RETCODE_LASTELEMENT);
  CheckTlv(tlv, tlvElement3);
}

TEST(MessageTest, MessageHandlerBufferOverloadTest)
{
  message::MessageStructure msg;
  msg.headerInfo.protocolVersion = 1;
  msg.headerInfo.res = 0;
  msg.token = 0;
  msg.messageType = message::MessageType::Command;
  msg.subType = message::MessageSubType::TESTCASE_SETUP;
  msg.errorCode = testapp::ErrCode::ERRCODE_TEST_CASE_ALREADY_REGISTERED;
  msg.testSection = 1;
  msg.testCase = 2;
  msg.testSuite = 3;
  msg.payloadLength = 0;

  uint8_t arr[200];
  message::TLV tlvElement;
  tlvElement.tag = 1;
  tlvElement.length = 200;
  tlvElement.value = arr;
  /*call the AddTlv method*/
  auto ret = message::addTLV(tlvElement, msg);
  EXPECT_EQ(ret, testapp::RetCode::RETCODE_SUCCESS);
  ret = message::addTLV(tlvElement, msg);
  EXPECT_EQ(ret, testapp::RetCode::RETCODE_FAILURE);
}

TEST(MessageTest, deserialize)
{
  constexpr uint8_t numberOfTestCases = 21u;
  std::array<std::array<uint8_t, 255>, numberOfTestCases> byteStream = {{
      {0x0, 0u, 1u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x01},
      {0x0, 0u, 2u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x02},
      {0x0, 0u, 3u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x03},
      {0x0, 0u, 11u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x04},
      {0x0, 0u, 12u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x05},
      {0x0, 0u, 13u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x06},
      {0x0, 0u, 21u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x07},
      {0x0, 0u, 22u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x08},
      {0x0, 0u, 23u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x09},
      {0x10, 0u, 0u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x0A},
      {0x10, 0u, 1u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x0B},
      {0x10, 0u, 2u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x0C},
      {0x20, 0u, 0u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x0D},
      {0x20, 0u, 1u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x0E},
      {0x10, 0u, 110u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x0F},
      {0x10, 0u, 112u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x10},
      {0x0, 0u, 0u, 0u, 0u, 1u, 0u, 8u, 'p','a','y','l','o','a','d',0x11},
      {0x0, 0u, 1u, 0u, 0u, 1u, 0u, 0u, },
      // Failure test cases:
      {0x0, 0u, 110u, 0u, 0u, 1u, 0u, 0u, },
      {0x10, 0u, 13u, 0u, 0u, 1u, 0u, 0u, },
      {0x20, 0u, 12u, 0u, 0u, 1u, 0u, 0u, }
  }};

  // Expected Deserialized message
  message::MessageStructure expectedMsg[numberOfTestCases] = {
    {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTSECTION_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x01}},
    {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTSUITE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x02}},
    {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTCASE_SETUP, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x03}},
    {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTSECTION_RUN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x04}},
    {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTSUITE_RUN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x05}},
    {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTCASE_RUN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x06}},
    {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTSECTION_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x07}},
    {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTSUITE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x08}},
    {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::TESTCASE_TEARDOWN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x09}},
    {{0, 1, 0}, message::MessageType::Report, 0u, message::MessageSubType::TEST_PASS, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x0A}},
    {{0, 1, 0}, message::MessageType::Report, 0u, message::MessageSubType::TEST_FAIL, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x0B}},
    {{0, 1, 0}, message::MessageType::Report, 0u, message::MessageSubType::TEST_NOTIMPLEMENTED, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x0C}},
    {{0, 2, 0}, message::MessageType::Acknowledgement, 0u, message::MessageSubType::ACKNOWLEDGEMENT, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x0D}},
    {{0, 2, 0}, message::MessageType::Acknowledgement, 0u, message::MessageSubType::NACK, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x0E}},
    {{0, 1, 0}, message::MessageType::Report, 0u, message::MessageSubType::REPORT_SUCCESS, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x0F}},
    {{0, 1, 0}, message::MessageType::Report, 0u, message::MessageSubType::REPORT_FAILURE, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x010}},
    {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::PING, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 8u, {'p','a','y','l','o','a','d',0x011}},
    {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::PONG, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 0u, {}},
    // Failure test cases:
    {{0, 0, 0}, message::MessageType::Command, 0u, message::MessageSubType::REPORT_SUCCESS, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 0u, {}},
    {{0, 1, 0}, message::MessageType::Report, 0u, message::MessageSubType::TESTCASE_RUN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 0u, {}},
    {{0, 2, 0}, message::MessageType::Acknowledgement, 0u, message::MessageSubType::TESTSUITE_RUN, testapp::ErrCode::SUCCESS, 0u, 1u, 0u, 0u, {}},
  };

  // Expected Return Code
  testapp::RetCode expectedRetCode[numberOfTestCases] = {
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      testapp::RetCode::RETCODE_SUCCESS,
      // Failure test cases:
      testapp::RetCode::RETCODE_INVALID_PARAM,
      testapp::RetCode::RETCODE_INVALID_PARAM,
      testapp::RetCode::RETCODE_INVALID_PARAM,
  };

  for (uint8_t iter = 0u; iter < numberOfTestCases; iter++)
  {
    message::MessageStructure msg;
    etl::array_view<uint8_t> byteStreamView(byteStream.at(iter));
    EXPECT_EQ(expectedRetCode[iter], message::deserialize(byteStreamView, msg)) << "Failure at test case # "<< (int16_t)iter+1;
    if(testapp::RetCode::RETCODE_SUCCESS == expectedRetCode[iter])
    {
      EXPECT_EQ(expectedMsg[iter].messageType, msg.messageType) << "Failure at test case # " << (int16_t)iter + 1;
      EXPECT_EQ(expectedMsg[iter].token, msg.token) << "Failure at test case # " << (int16_t)iter + 1;
      EXPECT_EQ(expectedMsg[iter].subType, msg.subType) << "Failure at test case # " << (int16_t)iter + 1;
      EXPECT_EQ(expectedMsg[iter].errorCode, msg.errorCode) << "Failure at test case # " << (int16_t)iter + 1;
      EXPECT_EQ(expectedMsg[iter].testSection, msg.testSection) << "Failure at test case # " << (int16_t)iter + 1;
      EXPECT_EQ(expectedMsg[iter].testSuite, msg.testSuite) << "Failure at test case # " << (int16_t)iter + 1;
      EXPECT_EQ(expectedMsg[iter].testCase, msg.testCase) << "Failure at test case # " << (int16_t)iter + 1;
      EXPECT_EQ(expectedMsg[iter].payloadLength, msg.payloadLength) << "Failure at test case # " << (int16_t)iter + 1;
      EXPECT_EQ(0, memcmp(&expectedMsg[iter].payload, &msg.payload, sizeof(msg.payload))) << "Failure at test case # " << (int16_t)iter + 1;
    }
  }
}
