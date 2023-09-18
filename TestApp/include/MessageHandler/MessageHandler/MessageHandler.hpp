/**
 * @file include/MessageHandler/MessageHandler/MessageHandler.hpp
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

#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include <cstdint>
#include <cstddef>
#include <array>
#include <etl/array_view.h>
#include "TestAppTypes/TestAppTypes.hpp"


namespace message
{
  /// @brief Maximum size of an FDX frame payload in bytes.
  constexpr uint8_t maxPayloadSize = 247u;
  constexpr uint8_t tlvHeaderSize = 2u;
  constexpr uint8_t messageSize = maxPayloadSize - tlvHeaderSize;
  constexpr uint8_t messageHeaderSize = 8u;
  constexpr uint8_t messageArraySize = 255u;

  constexpr uint8_t headerInfoIndex = 0U;
  constexpr uint8_t tokenIndex = 1U;
  constexpr uint8_t subTypeIndex = 2U;
  constexpr uint8_t errorCodeIndex = 3U;
  constexpr uint8_t testSectionIndex = 4U;
  constexpr uint8_t testSuiteIndex = 5U;
  constexpr uint8_t testCaseIndex = 6U;
  constexpr uint8_t payloadLengthIndex = 7U;
  constexpr uint8_t payloadIndex = 8U;

  constexpr uint8_t reportTlvType = 110u;

  /// @brief Identifies the message type of an FDX message
  enum MessageType : uint8_t
  {
    Command = 0u,
    Report,
    Acknowledgement,
    Log
  };

  ///Enum class Messagetype
  enum class MessageSubType : uint8_t
  {
    //types of messages to be handled
    //setup
    TESTSECTION_SETUP = 1, //NOLINT (readability-identifier-naming): we want keep googletest style
    TESTSUITE_SETUP = 2, //NOLINT (readability-identifier-naming): we want keep googletest style
    TESTCASE_SETUP = 3, //NOLINT (readability-identifier-naming): we want keep googletest style

    //run
    TESTSECTION_RUN = 11, //NOLINT (readability-identifier-naming): we want keep googletest style
    TESTSUITE_RUN = 12, //NOLINT (readability-identifier-naming): we want keep googletest style
    TESTCASE_RUN = 13, //NOLINT (readability-identifier-naming): we want keep googletest style

    //teardown
    TESTSECTION_TEARDOWN = 21, //NOLINT (readability-identifier-naming): we want keep googletest style
    TESTSUITE_TEARDOWN = 22, //NOLINT (readability-identifier-naming): we want keep googletest style
    TESTCASE_TEARDOWN = 23, //NOLINT (readability-identifier-naming): we want keep googletest style

    //type of messages received

    TEST_PASS = 0, //NOLINT (readability-identifier-naming): we want keep googletest style
    TEST_FAIL = 1, //NOLINT (readability-identifier-naming): we want keep googletest style
    TEST_NOTIMPLEMENTED = 2, //NOLINT (readability-identifier-naming): we want keep googletest style

    //possible ack messages
    ACKNOWLEDGEMENT = 0, //NOLINT (readability-identifier-naming): we want keep googletest style
    NACK = 1, //NOLINT (readability-identifier-naming): we want keep googletest style

    //REPORT
    REPORT_SUCCESS = 110, //NOLINT (readability-identifier-naming): we want keep googletest style
    REPORT_FAILURE = 112, //NOLINT (readability-identifier-naming): we want keep googletest style

    //PING->PONG
    PING = 0, //NOLINT (readability-identifier-naming): we want keep googletest style
    PONG = 1, //NOLINT (readability-identifier-naming): we want keep googletest style

    //LOG
    RESERVED = 0 //NOLINT (readability-identifier-naming): we want keep googletest style
  };

  // Arrays of fixed size, each array contains valid commands, reports or acknowledgment messages
  // Will be used to check the validity of the messages received from ITF
  constexpr std::array<MessageSubType, 10> msgCommands = {message::MessageSubType::PING,
            message::MessageSubType::TESTSECTION_SETUP, message::MessageSubType::TESTSUITE_SETUP, message::MessageSubType::TESTCASE_SETUP,
            message::MessageSubType::TESTSECTION_RUN, message::MessageSubType::TESTSUITE_RUN, message::MessageSubType::TESTCASE_RUN,
            message::MessageSubType::TESTSECTION_TEARDOWN, message::MessageSubType::TESTSUITE_TEARDOWN, message::MessageSubType::TESTCASE_TEARDOWN};

  constexpr std::array<MessageSubType, 5> msgReports = {message::MessageSubType::TEST_PASS, message::MessageSubType::TEST_FAIL, message::MessageSubType::TEST_NOTIMPLEMENTED,
            message::MessageSubType::REPORT_SUCCESS, message::MessageSubType::REPORT_FAILURE};

  constexpr std::array<MessageSubType, 2> msgAcknowledgements = {message::MessageSubType::ACKNOWLEDGEMENT, message::MessageSubType::NACK};

  #if defined(__LITTLE_ENDIAN__) || (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__))
  // Bit field used that can be used to read the first element of a header
  struct FirstS
  {
    uint8_t res : 4;
    uint8_t messageType : 2;
    uint8_t protocolVersion : 2;
  };
  #elif defined(__BIG_ENDIAN__) || (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__))
  // Bit field used that can be used to read the first element of a header
  struct FirstS
  {
    uint8_t protocolVersion : 2;
    uint8_t messageType : 2;
    uint8_t res : 4;
  };
  #else
  #error No suitbale endianess defined
  #endif

  using FirstT = struct FirstS;

  // TestApp generic Message type
  template <uint8_t maxPayloadSize>
  struct Message
  {
  /**
  Template with messagestruct, containing
  headerinfo -->version|message type|reserved
  MessageHeader-->subtype|errorcode|testsection|testsuite|testcase|payloadlength
  */
    FirstT headerInfo{};
    MessageType messageType{};
    uint8_t token{0u};
    MessageSubType subType{};
    testapp::ErrCode errorCode{};
    uint8_t testSection{0u};
    uint8_t testSuite{0u};
    uint8_t testCase{0u};
    uint8_t payloadLength{0u};
    std::array<uint8_t, maxPayloadSize> payload{};
  };

  /// Data type sent from TestRunner to Test Environment to process command
  struct TestExecutionMessage
  {
    MessageSubType subType{};
    void* testSuiteOrCase = nullptr;
  };

  struct TLV
  {
    uint8_t tag = 0;
    uint8_t length = 0;
    uint8_t *value = nullptr;
  };

  using MessageStructure = Message<maxPayloadSize>;
  using MessageBytes = std::array<uint8_t, messageArraySize>;

  /*functions for serializing, deserializing, parsing and adding TLV'S*/
  /*Deserialize bytestream to a message struct*/
  testapp::RetCode deserialize(const etl::array_view<uint8_t>& bytestream, MessageStructure &msg);

  /*Serialize messagestruct to bytearray*/
  testapp::RetCode serialize(const MessageStructure &msg, etl::array_view<uint8_t>& bytearray);

  /// Parse tlv element
  testapp::RetCode parseTLV(MessageStructure &msg, TLV &nextTlv);

  /*Add tlv element to the message structure*/
  testapp::RetCode addTLV(const TLV &nextTlv, MessageStructure &msg);

} // namespace message
#endif
