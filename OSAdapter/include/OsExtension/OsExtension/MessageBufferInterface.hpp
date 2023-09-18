/**
 * @file include/OsExtension/OsExtension/MessageBufferInterface.hpp
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

#ifndef OSEXTENSION_MESSAGEBUFFERINTERFACE_HPP
#define OSEXTENSION_MESSAGEBUFFERINTERFACE_HPP

#include <OsExtension/OsExtensionInterface.hpp>
#include <OsExtension/Semaphore.hpp>

#include <etl/array_view.h>

#include <cstddef>

namespace osextension {

/**
 * @brief Interface to a Message Buffer.
 */
class MessageBufferInterface : public OsExtensionInterface {
public:
  /// Constructor
  MessageBufferInterface() = default;

  /// Copy constructor
  MessageBufferInterface(MessageBufferInterface const&) = default;

  /// Move constructor
  MessageBufferInterface(MessageBufferInterface&&) noexcept = default;

  /// Copy assignment operator @return
  MessageBufferInterface& operator=(MessageBufferInterface const&) = default;

  /// Move assignment operator @return
  MessageBufferInterface& operator=(MessageBufferInterface&&) noexcept = default;

  /// Destructor
  ~MessageBufferInterface() override = default;

  /**
   * @brief Sends/puts a new element into the message buffer
   *
   * @param data pointer to buffer containing the data to be sent
   * @param length amount of bytes to be sent
   * @return number of bytes written into the message buffer
   */
  virtual size_t send(const void* data, size_t length) = 0;

  /**
   * @brief Sends/puts a new element into the message buffer, blocking on full buffer till timeout.
   * @param data pointer to buffer containing the data to be sent
   * @param length length amount of bytes to be sent
   * @param ticksToWait timeout in os systicks until the send/put is aborted
   * @return number of bytes written into the message buffer
   */
  virtual size_t send(const void* data, size_t length, TickType_t ticksToWait) = 0;

  /**
   * @brief Sends/puts a new element into the message buffer
   *
   * @param data pointer to buffer containing the data to be sent
   * @return number of bytes written into the message buffer
   */
  virtual size_t send(etl::array_view<const uint8_t> data) = 0;

  /**
   * @brief Sends/puts a new element into the message buffer, blocking on full buffer till timeout.
   * @param data pointer to buffer containing the data to be sent
   * @param ticksToWait timeout in os systicks until the send/put is aborted
   * @return number of bytes written into the message buffer
   */
  virtual size_t send(etl::array_view<const uint8_t> data, TickType_t ticksToWait) = 0;

  /**
   * @brief Sends/puts a new element into the message buffer from ISR context
   *
   * @param data pointer to buffer containing the data to be sent
   * @param length amount of bytes to be sent
   * @param higherPriorityTaskWoken is set to true if the send has lead to a context switch
   * @return number of bytes written into the message buffer
   */
  virtual size_t sendFromISR(const void* data, size_t length, BaseType_t* higherPriorityTaskWoken) = 0;

  /**
   * @brief Sends/puts a new element into the message buffer from ISR context
   *
   * @param data pointer to buffer containing the data to be sent
   * @param higherPriorityTaskWoken is set to true if the send has lead to a context switch
   * @return number of bytes written into the message buffer
   */
  virtual size_t sendFromISR(etl::array_view<const uint8_t> data, BaseType_t* higherPriorityTaskWoken) = 0;

  /**
   * @brief Receives/gets message from message buffer
   *
   * Note: This function does not support multiple receiving tasks
   *
   * @param data pointer to a buffer that will be filled with received bytes
   * @param length how many bytes to receive
   * @param xTicksToWait timeout in os systicks until the receive is aborted
   * @return number of bytes read from the message buffer
   */
  virtual size_t receive(void* data, size_t length, TickType_t xTicksToWait) = 0;

  /**
   * @brief Receives/gets message from message buffer
   *
   * Note: This function does not support multiple receiving tasks
   *
   * @param data pointer to a buffer that will be filled with received bytes
   * @param xTicksToWait timeout in os systicks until the receive is aborted
   * @return number of bytes read from the message buffer
   */
  virtual size_t receive(etl::array_view<uint8_t> data, TickType_t xTicksToWait) = 0;

  /**
   * @brief Receives/gets message from message buffer from ISR context
   *
   * Note: This function does not support multiple receiving tasks
   *
   * @param data pointer to a buffer that will be filled with received bytes
   * @param length how many bytes to receive
   * @param higherPriorityTaskWoken is set to true if the receiving forced a context switch
   * @return number of bytes read from the message buffer
   */
  virtual size_t receiveFromISR(void* data, size_t length, BaseType_t* higherPriorityTaskWoken) = 0;

  /**
   * @brief Receives/gets message from message buffer from ISR context
   *
   * Note: This function does not support multiple receiving tasks
   *
   * @param data pointer to a buffer that will be filled with received bytes
   * @param higherPriorityTaskWoken is set to true if the receiving forced a context switch
   * @return number of bytes read from the message buffer
   */
  virtual size_t receiveFromISR(etl::array_view<uint8_t> data, BaseType_t* higherPriorityTaskWoken) = 0;

  /**
   * @brief Resets message buffer and binary semaphore
   *
   * @retval true on sucess
   * @retval false on failure
   */
  virtual bool reset(void) = 0;

  /**
   * @brief Checks if message buffer is empty
   *
   * @retval true if queue is empty
   * @retval false if queue is not empty
   */
  virtual bool isEmpty(void) = 0;

  /**
   * @brief Checks if message buffer is full
   *
   * @retval true when queue is full
   * @retval false when queue is not full
   */
  virtual bool isFull(void) = 0;

  /**
   * @brief Number of available messages stored in the buffer
   *
   * @return number of messages
   */
  virtual size_t numMessagesAvailable() = 0;

  /**
   * @brief Number of available bytes stored in the buffer
   *
   * @return number of bytes
   */
  virtual size_t numBytesAvailable() = 0;
};

}  // namespace osextension

#endif /* OSEXTENSION_MESSAGEBUFFERINTERFACE_HPP */
