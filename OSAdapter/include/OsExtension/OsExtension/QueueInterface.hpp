/**
 * @file include/OsExtension/OsExtension/QueueInterface.hpp
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 */

#ifndef OSEXTENSION_QUEUEINTERFACE_HPP
#define OSEXTENSION_QUEUEINTERFACE_HPP

#include <cstdint>
#include <OsAbstraction/OsAbstraction.hpp>
#include <OsExtension/OsExtensionInterface.hpp>

namespace osextension {

/** Reassign native OS Type for readability and portability */
/* Note, this cannot be done inside a template since template types cannot be inherited */
using QueueTaskWokenTokenType = BaseType_t;

/**
 * @brief Interface to a Queue.
 *
 * @tparam ItemType Type for queue items
 */
template <typename ItemType>
class QueueInterface : public OsExtensionInterface {
public:
  /// Constructor
  QueueInterface() = default;

  /// Copy constructor
  QueueInterface(QueueInterface const&) = default;

  /// Move constructor
  QueueInterface(QueueInterface&&) noexcept = default;

  /// Copy assignment operator @return
  QueueInterface& operator=(QueueInterface const&) = default;

  /// Move assignment operator @return
  QueueInterface& operator=(QueueInterface&&) noexcept = default;

  /// Destructor
  ~QueueInterface() override = default;

  /**
   * @brief Sends a message to the back of the queue.
   * If timeout > 0, the task waits for space to become available on the
   * queue, should it be already full.
   * Timeout is only available in Task Context.
   *
   * @param[in] data   Pointer to the data to be sent
   * @param[in] timeout Time the task waits for free space in the queue
   *
   * @retval true Message sent
   * @retval false Message sending failed
   */
  virtual bool send(const ItemType& data, uint32_t timeout) = 0;

  /**
   * @brief Sends a message to the back of the queue.
   *
   * @param[in] data data to be sent
   *
   * @retval true Message sent
   * @retval false Message sending failed
   */
  virtual bool send(const ItemType& data) = 0;

  /**
   * @brief Sends a message to the back of the queue.
   * This Function is to be used in an ISR Context.
   *
   * @param[in] data data to be sent
   *
   * @retval true Message sent
   * @retval false Message sending failed
   */
  virtual bool sendFromIsr(const ItemType& data) = 0;

  /**
   * @brief Sends a message to the back of the queue.
   * This Function is to be used in an ISR Context.
   *
   * @param[in] data data to be sent
   * @param[out] taskWoken Token set to 1 if the send wakes a higher prior Task.
   *
   * @retval true Message sent
   * @retval false Message sending failed
   */
  virtual bool sendFromIsr(const ItemType& data, QueueTaskWokenTokenType* taskWoken) = 0;

  /**
   * @brief Receives a message from the queue.
   * If timeout > 0, the task waits for a new item until timeout occurs.
   *
   * @param[out] data   Pointer to data to be received
   * @param[in]  timeout Time the task waits for data
   *
   * @retval true Message received
   * @retval false Message receiving failed
   */
  virtual bool receive(ItemType& data, uint32_t timeout) = 0;

  /**
   * @brief Receives a message from the queue.
   * This method will block for an empty queue until a new message is available!
   * @see receive(ItemType& data, uint32_t timeout) to specify a timeout.
   *
   * @param[out] data data to be received
   *
   * @retval true Message received
   * @retval false Message receiving failed
   */
  virtual bool receive(ItemType& data) = 0;

  /**
   * @brief Receives a message from the queue in an ISR.
   * If no message is waiting to be read the call returns immediately.
   *
   * @param[out] data data to be received
   *
   * @retval true Message received
   * @retval false No message in Queue
   */
  virtual bool receiveFromIsr(ItemType& data) = 0;

  /**
   * @brief Evaluates number of items that the queue can still accommodate.
   *
   * @return Number of items that the queue can still accommodate.
   */
  virtual size_t spacesAvailable() = 0;
};

}  // namespace osextension

#endif  // OSEXTENSION_QUEUEINTERFACE_HPP
