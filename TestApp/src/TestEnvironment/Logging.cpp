/**
 * @file src/TestEnvironment/Logging.cpp
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

#include <TestEnvironment/Logging.hpp>

namespace testenvironment {

Logging& Logging::getInstance() {
  static Logging logging;
  return logging;
}

testapp::RetCode Logging::setMessageBuffer(osextension::MessageBufferReference& mbuf) {
  _testEnvironmentToTestRunnerMessageBufferReference = &mbuf;

  return testapp::RetCode::RETCODE_SUCCESS;
}

testapp::RetCode Logging::setDebugLog(bool isDebugLog) {
  _isDebugLog = isDebugLog;

  return testapp::RetCode::RETCODE_SUCCESS;
}

testapp::RetCode Logging::logInfo(const char* msg) {
  testapp::RetCode retCode = logGeneric(msg, testapp::MessageType::MESSAGE_INFO);

  return retCode;
}

testapp::RetCode Logging::logDebug(const char* msg) {
  testapp::RetCode retCode = logGeneric(msg, testapp::MessageType::MESSAGE_DEBUG);

  if (testapp::RetCode::RETCODE_SUCCESS == retCode && true != _isDebugLog) {
    // Error: isDebugLog not set
    retCode = testapp::RetCode::RETCODE_FAILURE;
  }

  return retCode;
}

testapp::RetCode Logging::logWarning(const char* msg) {
  testapp::RetCode retCode = logGeneric(msg, testapp::MessageType::MESSAGE_WARNING);

  return retCode;
}

testapp::RetCode Logging::logError(const char* msg) {
  testapp::RetCode retCode = logGeneric(msg, testapp::MessageType::MESSAGE_ERROR);

  return retCode;
}

testapp::RetCode Logging::logCritical(const char* msg) {
  testapp::RetCode retCode = logGeneric(msg, testapp::MessageType::MESSAGE_CRITICAL);

  return retCode;
}

testapp::RetCode Logging::logMessage(const char* msg) {
  testapp::RetCode retCode = logGeneric(msg, testapp::MessageType::MESSAGE_LOG);

  return retCode;
}

testapp::RetCode Logging::logGeneric(const char* msg, testapp::MessageType messageType){
  testapp::RetCode retCode = checkMessage(msg);

  if (testapp::RetCode::RETCODE_SUCCESS == retCode){
    sendMessage(msg, messageType);
  }

  if (testapp::RetCode::RETCODE_OUT_OF_RESOURCES == retCode) {
    const char* errMsg = "ERROR: MESSAGE TOO LONG";
    sendMessage(errMsg, messageType);
  }

  // catch failures
  if (retCode != testapp::RetCode::RETCODE_SUCCESS ||
      messageType == testapp::MessageType::MESSAGE_ERROR ||
      messageType ==  testapp::MessageType::MESSAGE_CRITICAL){
    _hasFailed = true;
  }

  return retCode;
}

testapp::RetCode Logging::checkMessage(const char* msg) {
  testapp::RetCode retCode = testapp::RetCode::RETCODE_SUCCESS;

  // Error: nullpointer
  if (nullptr == msg){
    retCode = testapp::RetCode::RETCODE_NULL_POINTER;
  }
  // Error: _testEnvironmentToTestRunnerMessageBufferReference not initialized
  else if (nullptr == _testEnvironmentToTestRunnerMessageBufferReference) {
    retCode = testapp::RetCode::RETCODE_UNINITIALIZED;
  }
  // Error: message too long
  else if (strlen(msg) + messageTypeStringsMaxSize >= testapp::messagebufferSize) {
    retCode = testapp::RetCode::RETCODE_OUT_OF_RESOURCES;
  }
  // Error: empty message
  else if (strlen(msg) <= 0) {
    retCode = testapp::RetCode::RETCODE_INVALID_PARAM;
  }

  return retCode;
}


void Logging::sendMessage(const char* msg, testapp::MessageType messageType) {
  testapp::LogMessageT log; 
  message::MessageStructure message{};
  message.messageType = message::MessageType::Log;
  message.subType = message::MessageSubType::RESERVED;
  
  auto messageTypeLength = strlen(_messageTypeStrings.at(messageType).value.data());
  auto payloadLength = strlen(msg) + messageTypeLength + messageTypeDelimiter.size();
  
  /* Construct tlv element message */
  // Message Format: <preliminary token>: <message text>
  // Add preliminary token; e.g. "DEBUG"
  std::copy_n(_messageTypeStrings.at(messageType).value.data(), messageTypeLength, log.text.begin());
  // Add Delimiter ": "
  std::copy_n(messageTypeDelimiter.begin(), messageTypeDelimiter.size(), log.text.begin() + messageTypeLength);
  // Add log message
  std::copy_n(msg, strlen(msg), log.text.begin() + messageTypeLength + messageTypeDelimiter.size());
  
  /* Add tlv element */
  message::TLV payloadInTlvFormat{message::reportTlvType, static_cast<uint8_t>(payloadLength), log.text.data()};
  message::addTLV(payloadInTlvFormat, message);
  
  /* send message to TestRunner */
  _testEnvironmentToTestRunnerMessageBufferReference->send(&message, sizeof(message), portMAX_DELAY);
}

void Logging::resetFailureFlag() {
  _hasFailed = false;
}

bool Logging::getFailureFlag() {
  return _hasFailed;
}

}  // namespace testenvironment
