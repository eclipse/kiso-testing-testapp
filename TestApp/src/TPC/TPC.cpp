/**
 * @file src/TPC/TPC/TPC.cpp
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

#include "TPC/TPC.hpp"
#include "TPC/TaskInterface.hpp"
#include <cstdint>
#include <array>
#include <etl/array_view.h>

constexpr uint8_t crcDefault = UINT8_C(0);
constexpr uint16_t crcBitShift4 = UINT16_C(4);   /** 4 bit shifting for CRC calculation */
constexpr uint16_t crcBitShift5 = UINT16_C(5);   /** 5 bit shifting for CRC calculation */
constexpr uint16_t crcBitShift8 = UINT16_C(8);   /** 8 bit shifting for CRC calculation */
constexpr uint16_t crcBitShift12 = UINT16_C(12); /** 12 bit shifting for CRC calculation */
constexpr uint16_t crcByteMask = UINT16_C(0xFF);
constexpr uint8_t ackNackMessageLength = 10U;
constexpr uint8_t crcSize = 2U;
constexpr uint16_t tpcBufferSize = 256U;

std::array<uint8_t, tpcBufferSize> buf{};
etl::array_view<uint8_t> bufView{buf};

namespace tpcprotocol
{
  static bool receiveNextMessage = true;

   /// set tPC message Buffer references outgoing
  void TPC::setTPCToTestRunnerMessageBufferReference(osextension::MessageBufferReference& mbf) {
    _tpcToTestRunnerMessageBuffer = &mbf;
  }

   /// set tPC message Buffer references incoming
  void TPC::setTestRunnerTPCMessageBufferReference(osextension::MessageBufferReference& mbf) {
    _testRunnerToTPCMessageBuffer = &mbf;
  }

  void TPC::oneCycle() {
    // initialization of the global variable storing the received data
    message::Message<message::maxPayloadSize> dataReceivedFromITF{};
    message::Message<message::maxPayloadSize> dataReceivedFromTestRunner{};
    size_t receivedBytesFromTestRunner = 0u;
    size_t receivedBytesFromITF = 0;

    if (tccinterface::tccInterface != nullptr) {
      tccinterface::tccInterface->receive(&receivedBytesFromITF, bufView);
    }

    if (receivedBytesFromITF > 0) {
      // Resize the buffer view (Does not copy the elements in the range).
      bufView.assign(bufView.begin(), receivedBytesFromITF);

      // check if the message is valid
      if (validateITFByteStream(bufView, receivedBytesFromITF) && bufView.size() == receivedBytesFromITF) {

        // Deserialize ITF bytestream to a MessageStructure for further processing
        if (testapp::RetCode::RETCODE_SUCCESS == message::deserialize(bufView, dataReceivedFromITF)) {
          // ITF has sent a command to be processed further by TestRunner and TestEnvironment
          if ((dataReceivedFromITF.messageType != message::MessageType::Acknowledgement) && receiveNextMessage) {

            // Send an Acknowledgement back to ITF
            sendACKorNACK(message::MessageSubType::ACKNOWLEDGEMENT, dataReceivedFromITF.token);

            // Process ITF command
            if (dataReceivedFromITF.subType != message::MessageSubType::PING) {
              // Send data to TestRunner to be processed
              _tpcToTestRunnerMessageBuffer->send(&dataReceivedFromITF, sizeof(dataReceivedFromITF));

              // Receive Log/report messages from TestRunner after proccess
              receivedBytesFromTestRunner = _testRunnerToTPCMessageBuffer->receive(&dataReceivedFromTestRunner, sizeof(dataReceivedFromTestRunner), portMAX_DELAY);

              // send the Log/report message to ITF
              if (receivedBytesFromTestRunner > 0){
                sendReportToITF(dataReceivedFromTestRunner, message::messageHeaderSize + dataReceivedFromTestRunner.payloadLength);
              }

              // In case of empty TestCase/TestSuite
              receiveNextMessage = dataReceivedFromTestRunner.messageType == message::MessageType::Report ? true : false;
            }
          }
          // The last command received from ITF is still in progress, and ITF sends Acknowledgement for the previously received Log or report
          else if (dataReceivedFromITF.messageType == message::MessageType::Acknowledgement && !receiveNextMessage) {

            // wait for Log messages or final report message.
            receivedBytesFromTestRunner = _testRunnerToTPCMessageBuffer->receive(&dataReceivedFromTestRunner, sizeof(dataReceivedFromTestRunner), portMAX_DELAY);

            // send the Log/report message to ITF
            if (receivedBytesFromTestRunner > 0) {
              sendReportToITF(dataReceivedFromTestRunner, message::messageHeaderSize + dataReceivedFromTestRunner.payloadLength);

              // when the final report is received, TPC sends the report to ITF and shall process the next command (command message type) coming from ITF.
              if (dataReceivedFromTestRunner.messageType == message::MessageType::Report){
                receiveNextMessage = true;
              }
            }
          }
          else {
            // > when the report has been sent to ITF, further Acknowledgement messages from ITF will be ignored.
            // > Receive and process next message.
          }
        }
      }
      else {
        // invalid message from ITF; send back Nack.
        sendACKorNACK(message::MessageSubType::NACK, bufView.at(message::tokenIndex));
      }
    }
  }

  /**
   * @brief ValidateITFByteStream Method to validate CRC for the bytestream
   * Compares the received and calculatedCRC values
   * returns a boolen if it is valid|invalid
   */
  bool TPC::validateITFByteStream(const etl::array_view<uint8_t>& bytestream, uint8_t length)
  {    
    bool ret = false;
    const uint8_t metadataLength = 10U; // 8 header info + 2 crc

    if (length >= metadataLength)
    {
      //possibility to receive multiple messages on RTT segger channel on receive api call
      //Fix to process only the first message received and discard other
      if(length > (bytestream.at(message::payloadLengthIndex) + metadataLength)){
        length = bytestream.at(message::payloadLengthIndex) + metadataLength;
      }    
      
      uint16_t receivedCRC = 0;
      receivedCRC = bytestream.at(length - 1);
      receivedCRC <<= crcBitShift8;
      receivedCRC += bytestream.at(length - 2);

      /*calculate the crc for the bytestream based on its length*/
      uint16_t calculatedCRC = 0;
      //etl::array_view<uint8_t> bytestreamView{reinterpret_cast<uint8_t*>(&message), sizeof(message)};
      calculatedCRC = calculateCRC(bytestream, length - crcSize);

      /*check if receivedcrc = calcCRC*/
      if (receivedCRC == calculatedCRC){
        ret = true;
      }
      else{
        ret = false;
      }
    }
    return (ret);
  }

  /**
   * @brief:To send AC|NACK to ITF
   * > construct an ack message
   * > add the crc to the bytearray
   * > send via fdx/RTT to ITF
   */
  testapp::RetCode TPC::sendACKorNACK(message::MessageSubType subType, uint8_t tokenID)
  {
    std::array<uint8_t, ackNackMessageLength> byteArray = {0u};
    etl::array_view<uint8_t> byteArrayView{byteArray};
    //construct ACK|NACKmsg

    message::MessageStructure ackMessage;
    ackMessage.headerInfo.protocolVersion = 1;
    ackMessage.headerInfo.messageType = message::MessageType::Acknowledgement;
    ackMessage.token = tokenID;
    ackMessage.subType = subType;

    /*Serialize the struct to bytearray to send it to ITF
    Calculate crc and add crc*/
    auto reportSerialize = message::serialize(ackMessage, byteArrayView);
    if(reportSerialize != testapp::RetCode::RETCODE_SUCCESS){
      return testapp::RetCode::RETCODE_FAILURE;
    }

    auto bareMessageLength = ackNackMessageLength - crcSize;
    auto crcAck = calculateCRC(byteArrayView, bareMessageLength);

    //AddCRC to the bytearray
    byteArrayView.at(bareMessageLength) = static_cast<uint8_t>(crcAck & crcByteMask);
    byteArrayView.at(bareMessageLength + 1) = static_cast<uint8_t>((crcAck >> crcBitShift8) & crcByteMask);

    //callfdx sendapi() with bytearray pointer and length
    if(tccinterface::tccInterface != nullptr){
      tccinterface::tccInterface->send(byteArrayView);
    }

    return testapp::RetCode::RETCODE_SUCCESS;
  }

  /**
   * @brief : Serializes the messagestruct to bytearray
    adds CRC to the bytearray
    Calls the fdxsend method to send the report
   *
   */
  testapp::RetCode TPC::sendReportToITF(const message::MessageStructure &reportMsg, uint8_t reportLength)
  {
    std::array<uint8_t, tpcBufferSize> byteArrayReport = {0u};
    etl::array_view<uint8_t> reportView{byteArrayReport.begin(), reportLength + crcSize};

    auto reportSerialize = message::serialize(reportMsg, reportView);
    if(reportSerialize != testapp::RetCode::RETCODE_SUCCESS){
      return testapp::RetCode::RETCODE_FAILURE;
    }

    auto crcReport = calculateCRC(reportView, reportLength);
    //send byteArray to ITF
    //add crc to the reportmsg
    reportView.at(reportLength) = static_cast<uint8_t>(crcReport & crcByteMask);
    reportView.at(reportLength + 1) = static_cast<uint8_t>((crcReport >> crcBitShift8) & crcByteMask);

    //callfdx sendapi() with bytearray pointer and length
    if(tccinterface::tccInterface != nullptr){
      tccinterface::tccInterface->send(reportView);
    }
    //send byteArray to kiso host

    return testapp::RetCode::RETCODE_SUCCESS;
  }

  /**
   * @brief :  Method to calculateCRC
   *
   */
  uint16_t TPC::calculateCRC(const etl::array_view<uint8_t>& buffer, uint8_t length)
  {
    uint16_t crc = crcDefault;

    for (uint8_t i = 0; i < length; i++)
    {
      crc = (crc >> (crcBitShift8)) | (crc << (crcBitShift8));
      crc ^= buffer.at(i);
      crc ^= (crc & (crcByteMask)) >> (crcBitShift4);
      crc ^= crc << (crcBitShift12);         
      crc ^= (crc & (crcByteMask)) << (crcBitShift5);
    }
    return crc;
  }

} // namespace tpcprotocol
