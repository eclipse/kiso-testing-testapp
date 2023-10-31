/**
 * @file include/OsExtension/OsExtension/Queue.hpp
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 */

#ifndef OSEXTENSION_QUEUE_HPP
#define OSEXTENSION_QUEUE_HPP

#include <cstdint>
#include <OsAbstraction/OsAbstraction.hpp>
#include <OsExtension/QueueInterface.hpp>
#include <array>

namespace osextension {

/**
 * @brief Reference to a FreeRTOS Queue
 *
 * This class is lightweight reference to a FreeRTOS queue which can be copied
 * to have a thread & MPU safe access to the queue.
 *
 * @tparam ItemType Type for queue items
 */
template <typename ItemType>
class QueueReference : public QueueInterface<ItemType> {
public:
  /// Constructor
  QueueReference() = default;

  /// Copy constructor
  QueueReference(QueueReference const&) = default;

  /// Move constructor
  QueueReference(QueueReference&&) noexcept = default;

  /// Copy assignment operator @return
  QueueReference& operator=(QueueReference const&) = default;

  /// Move assignment operator @return
  QueueReference& operator=(QueueReference&&) noexcept = default;

  /// Destructor
  ~QueueReference() override = default;

  /// @copydoc QueueInterface::send(const ItemType&, uint32_t)
  bool send(const ItemType& data, uint32_t timeout) override {
    return (osabstraction::queueSend(_handle, &data, timeout) == pdTRUE);
  }

  /// @copydoc QueueInterface::send(const ItemType&)
  bool send(const ItemType& data) override {
    return (osabstraction::queueSend(_handle, &data, portMAX_DELAY) == pdTRUE);
  }

  /// @copydoc QueueInterface::sendFromIsr(const ItemType&)
  bool sendFromIsr(const ItemType& data) override {
    return (osabstraction::queueSendFromISR(_handle, &data, nullptr) == pdTRUE);
  }

  /// @copydoc QueueInterface::sendFromIsr(const ItemType&, QueueTaskWokenTokenType*)
  bool sendFromIsr(const ItemType& data, QueueTaskWokenTokenType* taskWoken) override {
    return (osabstraction::queueSendFromISR(_handle, &data, taskWoken) == pdTRUE);
  }

  /// @copydoc QueueInterface::receive(ItemType&, uint32_t)
  bool receive(ItemType& data, uint32_t timeout) override {
    return (osabstraction::queueReceive(_handle, &data, timeout) == pdTRUE);
  }

  /// @copydoc QueueInterface::receive(ItemType&)
  bool receive(ItemType& data) override {
    return (osabstraction::queueReceive(_handle, &data, portMAX_DELAY) == pdTRUE);
  }

  /// @copydoc QueueInterface::receiveFromIsr(ItemType&)
  bool receiveFromIsr(ItemType& data) override {
    return (osabstraction::queueReceiveFromISR(_handle, &data, nullptr) == pdTRUE);
  }

  /// @copydoc QueueInterface::spacesAvailable()
  size_t spacesAvailable() override {
    return (osabstraction::queueSpacesAvailable(_handle));
  }

  /// @copydoc OsExtensionInterface::getHandleForQueueSet()
  OsExtensionInterface::OsExtensionHandleType getHandleForQueueSet() const override {
    return _handle;
  }

  /**
   * @brief Get the internal FreeRTOS queue handle
   *
   * @return FreeRTOS queue handle
   */
  [[deprecated("Copy object to share references or use getHandleForQueueSet() if you need add to FreeRTOS queue set.")]] QueueHandle_t getHandle() const {
    return _handle;
  }

protected:
  /**
   * @brief Get the internal FreeRTOS queue handle
   *
   * @return FreeRTOS queue handle
   */
  QueueHandle_t getInternalHandle() const {
    return _handle;
  }

  /**
   * @brief Set the internal FreeRTOS queue handle
   *
   * @param[in] handle handle
   */
  void setHandle(QueueHandle_t handle) {
    _handle = handle;
  }

private:
  /// Internal FreeRTOS handle
  QueueHandle_t _handle{nullptr};
};

/**
 * @brief FreeRTOS Queue with external storage buffer
 *
 * This class is encapsulates a FreeRTOS queue where the storage buffer for
 * the FreeRTOS queue is outside of the object.
 *
 * @tparam ItemType Type for queue items
 * @tparam length Length (number of items) of queue
 */
template <typename ItemType, size_t length>
class QueueWithExternalMemory : public QueueReference<ItemType> {
public:
  /**
   * @brief Constructs queue with external memory
   *
   * @param[in] storageBuffer Reference to storage buffer of the queue
   * @param[in] name Queue name or nullptr if no name is desired. Pointer must
   *                 be valid for the life-time of the this object.
   */
  explicit QueueWithExternalMemory(std::array<ItemType, length>& storageBuffer, const char* name = nullptr) {
    auto* const handle = osabstraction::queueCreateStatic(length, sizeof(ItemType), reinterpret_cast<uint8_t*>(&storageBuffer), &_buffer);  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast) reinterpret_cast required due to external API

    this->setHandle(handle);
    if ((handle != nullptr) and (name != nullptr)) {
      /* We want this queue to be viewable in a RTOS kernel aware debugger,
      so register it. */
      osabstraction::queueRegister(handle, name);
    }
  }

  /// Deleted copy constructor
  QueueWithExternalMemory(const QueueWithExternalMemory&) = delete;

  /// Deleted move constructor
  QueueWithExternalMemory(QueueWithExternalMemory&&) = delete;

  /// Delete copy assignment operator @return
  QueueWithExternalMemory operator=(const QueueWithExternalMemory&) = delete;

  /// Deleted move assignment operator @return
  QueueWithExternalMemory& operator=(QueueWithExternalMemory&&) = delete;

  /// Destructor
  ~QueueWithExternalMemory() override {
    osabstraction::queueUnregister(this->getInternalHandle());
    osabstraction::queueDelete(this->getInternalHandle());
  }

  /**
   * @brief Gets a reference to this queue
   * @return Reference to this queue
   */
  QueueReference<ItemType> getReference() const {
    return QueueReference<ItemType>(*this);  //NOLINT(cppcoreguidelines-slicing) this is exactly what we want here
  }

  /**
   * @brief Gets a reference to this queue
   * @return Reference to this queue
   */
  [[deprecated("Use getReference().")]] QueueReference<ItemType> getQueueReference() const {
    return getReference();
  }

private:
  /// Static queue buffer
  StaticQueue_t _buffer{};
};

/**
 * @brief FreeRTOS Queue with internal storage buffer
 *
 * This class is encapsulates a FreeRTOS queue where the storage buffer for
 * the FreeRTOS queue is inside of the object.
 *
 * @tparam ItemType Type for queue items
 * @tparam length Length (number of items) of queue
 */
template <typename ItemType, size_t length>
class Queue : public QueueWithExternalMemory<ItemType, length> {
public:
  /**
   * @brief: Constructs queue
   *
   * @param[in] name Queue name or nullptr if no name is desired. Pointer must
   *                 be valid for the life-time of the this object.
   */
  explicit Queue(const char* name = nullptr) : QueueWithExternalMemory<ItemType, length>{_storageBuffer, name} {}

  /// Deleted copy constructor
  Queue(const Queue&) = delete;

  /// Deleted move constructor
  Queue(Queue&&) = delete;

  /// Delete copy assignment operator @return
  Queue operator=(const Queue&) = delete;

  /// Deleted move assignment operator @return
  Queue& operator=(Queue&&) = delete;

  /// Destructor
  ~Queue() override = default;

private:
  /// Storage buffer
  std::array<ItemType, length> _storageBuffer{};
};

}  // namespace osextension

#endif  // OSEXTENSION_QUEUE_HPP
