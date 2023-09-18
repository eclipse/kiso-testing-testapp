/**
 * @file include/TestEnvironment/TestEnvironment/Logging.hpp
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

#ifndef TESTENVIRONMENT_LOGGING_HPP
#define TESTENVIRONMENT_LOGGING_HPP

#include <array>

#include <TestEnvironment/LoggingInterface.hpp>
#include <MessageHandler/MessageHandler.hpp>

#include <OsExtension/MessageBuffer.hpp>
#include <etl/map.h>

/**
 * @brief: testenvironment namespace
 */
namespace testenvironment {
  constexpr uint8_t errorTypes = 6u;
  constexpr uint8_t messageTypeStringsMaxSize = 8u;
  constexpr uint8_t delimiterStringSize = 2u;
  constexpr std::array<char, delimiterStringSize> messageTypeDelimiter{':', ' '};
  struct MessageTypeString {
    std::array<char, messageTypeStringsMaxSize+1> value;
  };
  constexpr struct {MessageTypeString info, debug, warning, error, critical, log;} messageTypeString = {
    {"INFO"}, {"DEBUG"}, {"WARNING"}, {"ERROR"}, {"CRITICAL"}, {"LOG"}
  };

/**
 * @brief Logging
 */
class Logging : public LoggingInterface {
public:
  /* Deleted copy constructor */
  Logging(const Logging&) = delete;
  /* Deleted move constructor */
  Logging(const Logging&&) = delete;
  /* Deleted copy assignment */
  Logging& operator=(const Logging&) = delete;
  /* Deleted move assignment*/
  Logging& operator=(Logging&&) = delete;
  /**
   * @brief Get the Instance object
   *
   * @return Logging&
   */
  static Logging& getInstance();

  /**
   * @brief Set the Message Buffer object
   *
   * @param mbuf
   * @return testapp::RetCode_T::RETCODE_SUCCESS
   */
  testapp::RetCode setMessageBuffer(osextension::MessageBufferReference& mbuf);

  /**
   * @brief Set the Debug Log object
   *
   * @param isDebugLog
   * @return testapp::RetCode_T::RETCODE_SUCCESS
   */
  testapp::RetCode setDebugLog(bool isDebugLog);
  testapp::RetCode logInfo(const char* msg) override;
  testapp::RetCode logDebug(const char* msg) override;
  testapp::RetCode logWarning(const char* msg) override;
  testapp::RetCode logError(const char* msg) override;
  testapp::RetCode logCritical(const char* msg) override;
  testapp::RetCode logMessage(const char* msg) override;

  /**
   * @brief catch failures while logging messages.
   * @param None
   * @return bool
   */
  bool getFailureFlag() override;

  /**
   * @brief reset logging flag _hasFailed; the flag could be set to true when an errorLog
   * or criticalLog was invoked or when other issues such as msg too long occur.
   * @param None
   * @return None
   */
  void resetFailureFlag() override;

  /**
   * @brief Sends the message
   *
   * @param msg
   * @param messageType
   */
  void sendMessage(const char* msg, testapp::MessageType messageType);

private:
  /**
   * @brief To prevent repetitve code, logGeneric calls checkMessage
   * and sends the corresponding log message and type.
   *
   * @param msg
   * @param messageType
   * @return testapp::RetCode::RETCODE_SUCCESS
   * @return testapp::RetCode::RETCODE_NULL_POINTER (msg is nullpointer)
   * @return testapp::RetCode::RETCODE_UNINITIALIZED (_testEnvironmentToTestRunnerMessageBufferReference unitialized)
   * @return testapp::RetCode::RETCODE_OUT_OF_RESOURCES (msg too long)
   * @return testapp::RetCode::RETCODE_INVALID_PARAM (empty msg)
   */
  testapp::RetCode logGeneric(const char* msg, testapp::MessageType messageType);

  /**
   * @brief Checks whether it is a valid message
   *
   * @param msg
   * @return testapp::RetCode::RETCODE_SUCCESS
   * @return testapp::RetCode::RETCODE_NULL_POINTER (msg is nullpointer)
   * @return testapp::RetCode::RETCODE_UNINITIALIZED (_testEnvironmentToTestRunnerMessageBufferReference unitialized)
   * @return testapp::RetCode::RETCODE_OUT_OF_RESOURCES (msg too long)
   * @return testapp::RetCode::RETCODE_INVALID_PARAM (empty msg)
   */
  testapp::RetCode checkMessage(const char* msg);
  
  /// A map of MessageTpes to charachters
  const etl::map<testapp::MessageType, MessageTypeString, errorTypes> _messageTypeStrings = {
    {testapp::MessageType::MESSAGE_INFO, messageTypeString.info},     
    {testapp::MessageType::MESSAGE_DEBUG, messageTypeString.debug},    
    {testapp::MessageType::MESSAGE_WARNING, messageTypeString.warning},  
    {testapp::MessageType::MESSAGE_ERROR, messageTypeString.error},    
    {testapp::MessageType::MESSAGE_CRITICAL, messageTypeString.critical}, 
    {testapp::MessageType::MESSAGE_LOG, messageTypeString.log} 
  };

  /// Message Buffer reference for Outgoing messages from testEnvironment to TestRunner
  osextension::MessageBufferReference* _testEnvironmentToTestRunnerMessageBufferReference = nullptr;  // Queues are Message buffers
  bool _isDebugLog = false;
  bool _hasFailed = false;

  Logging() = default;
  ~Logging() = default;

};

}  // namespace testenvironment
#endif  // TESTENVIRONMENT_LOGGING_HPP
