/**
 * @file src/MessageHandler/MessageHandler.cpp
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

#include "MessageHandler/MessageHandler.hpp"
#include <cstring>
#include <algorithm>

namespace message
{
  /*Method to deserialize bytearray to message struct*/
  testapp::RetCode deserialize(const etl::array_view<uint8_t>& bytestream, MessageStructure &msg)
  {
    // msg.headerInfo.protocolVersion = ((bytestream[0] & 0xC0)  >> 6);
    // msg.headerInfo.res = (bytestream[0] & 0x0F);
    const uint8_t messageTypeMask = 0x30U;
    const uint8_t messageTypeBitOffset = 4U;

    std::memcpy(&msg.headerInfo, &bytestream.at(headerInfoIndex), sizeof(msg.headerInfo));
    msg.messageType = static_cast<MessageType>((bytestream.at(headerInfoIndex) & messageTypeMask) >> messageTypeBitOffset);
    msg.token = bytestream.at(tokenIndex);

    msg.subType = static_cast<MessageSubType>(bytestream.at(subTypeIndex));
    switch (msg.messageType)
    {
      case message::MessageType::Command:
        if( std::find(msgCommands.begin(), msgCommands.end(), msg.subType) == msgCommands.end() )
        {
          return testapp::RetCode::RETCODE_INVALID_PARAM;
        }
        break;
      case message::MessageType::Report:
        if( std::find(msgReports.begin(), msgReports.end(), msg.subType) == msgReports.end() )
        {
          return testapp::RetCode::RETCODE_INVALID_PARAM;
        }
        break;
      case message::MessageType::Acknowledgement:
        if( std::find(msgAcknowledgements.begin(), msgAcknowledgements.end(), msg.subType) == msgAcknowledgements.end() )
        {
          return testapp::RetCode::RETCODE_INVALID_PARAM;
        }
        break;
      default:
        return testapp::RetCode::RETCODE_INVALID_PARAM;
        break;
    }

    msg.errorCode = static_cast<testapp::ErrCode>(bytestream.at(errorCodeIndex));
    msg.testSection = bytestream.at(testSectionIndex);
    msg.testSuite = bytestream.at(testSuiteIndex);
    msg.testCase = bytestream.at(testCaseIndex);
    msg.payloadLength = bytestream.at(payloadLengthIndex);
    std::memcpy(&msg.payload, &bytestream.at(payloadIndex), msg.payloadLength);

    return testapp::RetCode::RETCODE_SUCCESS;
  }

  /*Method to serialize messagestruct to bytearray*/
  testapp::RetCode serialize(const MessageStructure &msg, etl::array_view<uint8_t>& bytestream)
  {
    std::memcpy(&bytestream.at(headerInfoIndex), &msg.headerInfo, sizeof(msg.headerInfo));
    bytestream.at(tokenIndex) = msg.token;
    bytestream.at(subTypeIndex) = static_cast<uint8_t>(msg.subType);
    bytestream.at(errorCodeIndex) = static_cast<uint8_t>(msg.errorCode);
    bytestream.at(testSectionIndex) = msg.testSection;
    bytestream.at(testSuiteIndex) = msg.testSuite;
    bytestream.at(testCaseIndex) = msg.testCase;
    bytestream.at(payloadLengthIndex) = msg.payloadLength;
    
    std::memcpy(&bytestream.at(payloadIndex), &msg.payload, msg.payloadLength);

    return testapp::RetCode::RETCODE_SUCCESS;
  }

  testapp::RetCode parseTLV(MessageStructure &msg, TLV &nextTlv)
  {
    static uint8_t nextTLVPlace = 0;

    // Find the next TLV element
    nextTlv.tag = msg.payload[nextTLVPlace];
    nextTlv.length = msg.payload[nextTLVPlace + 1];
    nextTlv.value = &msg.payload[nextTLVPlace + 2];

    if (nextTLVPlace + (tlvHeaderSize + nextTlv.length) > msg.payloadLength)
    {
      // Somebody tried to escape its scope
      nextTlv.tag = nextTlv.length = 0;
      nextTlv.value = nullptr;
    }
    // Allocate the right nextTLVPlace
    nextTLVPlace += tlvHeaderSize + nextTlv.length;

    // Check validity of the nextTLVPlace
    if (nextTLVPlace >= msg.payloadLength)
    {
      nextTLVPlace = 0;
    }

    // Return the right error
    if (nextTLVPlace == 0)
    {
      return testapp::RetCode::RETCODE_LASTELEMENT;
    }
    else
    {
      return testapp::RetCode::RETCODE_SUCCESS;
    }
  }

  /*Add tlv element to the message structure*/
  testapp::RetCode addTLV(const TLV &nextTlv, MessageStructure &msg)
  {
    if (nextTlv.length > messageSize)
    {
      return testapp::RetCode::RETCODE_FAILURE;
    }

    if (nextTlv.length + tlvHeaderSize + msg.payloadLength > maxPayloadSize)
    {
      return testapp::RetCode::RETCODE_FAILURE;
    }

    msg.payload[msg.payloadLength] = nextTlv.tag;
    msg.payload[msg.payloadLength + 1] = nextTlv.length;

    memcpy(&msg.payload[msg.payloadLength + tlvHeaderSize], nextTlv.value, nextTlv.length);

    msg.payloadLength = msg.payloadLength + nextTlv.length + tlvHeaderSize;

    return testapp::RetCode::RETCODE_SUCCESS;
  }
} // namespace message
