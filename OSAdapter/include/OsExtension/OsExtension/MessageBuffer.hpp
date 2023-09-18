/**
 * @file include/OsExtension/OsExtension/MessageBuffer.hpp
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 */

#ifndef OSEXTENSION_MESSAGEBUFFER_HPP
#define OSEXTENSION_MESSAGEBUFFER_HPP

#include <Observer/Observer.hpp>
#include <OsExtension/MessageBufferInterface.hpp>
#include <OsExtension/Semaphore.hpp>

#include <etl/array_view.h>

#include <cstddef>

namespace osextension {

/**
 * Event indicating that data has been added to the given buffer
 */
struct DataAddedEvent {

  /// buffer to which data has been added
  MessageBufferInterface& buffer;

  /// pointer to the data added
  const void* data;

  /// length of the added data in number of bytes
  const size_t length;
};

/**
 * Event indicating that data has been removed from given buffer
 */
struct DataRemovedEvent {

  /// buffer to which data has been added
  MessageBufferInterface& buffer;

  /// pointer to the data removed
  const void* data;

  /// length of the data removed in number of bytes
  const size_t length;
};

/**
 * Event indicating that data sent to the buffer is dropped due to a full buffer
 */
struct DataDroppedEvent {
  /// buffer which dropped the data
  MessageBufferInterface& buffer;

  /// pointer to the dropped data
  const void* data;

  /// length of the data dropped in number of bytes
  const size_t length;
};

/// Type alias for MessageBuffer observers
using MessageBufferObserver = observer::Observer<DataAddedEvent, DataRemovedEvent, DataDroppedEvent>;

/// Type alias for observable MessageBuffer implementations
using MessageBufferObservable = observer::Observable<MessageBufferObserver>;

/**
 * @brief Reference to a FreeRTOS Message Buffer
 *
 * This class is lightweight reference to a FreeRTOS queue which can be copied
 * to have a thread & MPU safe access to the queue.
 * It can be observed by attaching an MessageBufferObserver to listen to addition
 * or deletions of messages (@see MessageBufferMonitor.hpp)
 */
class MessageBufferReference : public MessageBufferInterface,
                               public MessageBufferObservable {
public:
  /// Constructor
  MessageBufferReference() = default;

  /// Copy constructor
  MessageBufferReference(MessageBufferReference const&) = default;

  /// Move constructor
  MessageBufferReference(MessageBufferReference&&) noexcept = default;

  /// Copy assignment operator @return
  MessageBufferReference& operator=(MessageBufferReference const&) = default;

  /// Move assignment operator @return
  MessageBufferReference& operator=(MessageBufferReference&&) noexcept = default;

  /// Destructor
  ~MessageBufferReference() override = default;

  /// @copydoc MessageBufferInterface::send(const void*, size_t)
  size_t send(const void* data, size_t length) override;

  /// @copydoc MessageBufferInterface::send(const void*, size_t, TickType_t)
  size_t send(const void* data, size_t length, TickType_t ticksToWait) override;

  /// @copydoc MessageBufferInterface::send(etl::array_view<const uint8_t>)
  size_t send(etl::array_view<const uint8_t> data) override;

  /// @copydoc MessageBufferInterface::send(etl::array_view<const uint8_t>, TickType_t)
  size_t send(etl::array_view<const uint8_t> data, TickType_t ticksToWait) override;

  /// @copydoc MessageBufferInterface::sendFromISR(const void*, size_t, BaseType_t*)
  size_t sendFromISR(const void* data, size_t length, BaseType_t* higherPriorityTaskWoken) override;

  /// @copydoc MessageBufferInterface::sendFromISR(etl::array_view<const uint8_t>, BaseType_t*)
  size_t sendFromISR(etl::array_view<const uint8_t> data, BaseType_t* higherPriorityTaskWoken) override;

  /// @copydoc MessageBufferInterface::receive(void*, size_t, TickType_t)
  size_t receive(void* data, size_t length, TickType_t xTicksToWait) override;

  /// @copydoc MessageBufferInterface::receive(etl::array_view<uint8_t>, TickType_t)
  size_t receive(etl::array_view<uint8_t> data, TickType_t xTicksToWait) override;

  /// @copydoc MessageBufferInterface::receiveFromISR(void*, size_t, BaseType_t*)
  size_t receiveFromISR(void* data, size_t length, BaseType_t* higherPriorityTaskWoken) override;

  /// @copydoc MessageBufferInterface::receiveFromISR(etl::array_view<uint8_t>, BaseType_t*)
  size_t receiveFromISR(etl::array_view<uint8_t> data, BaseType_t* higherPriorityTaskWoken) override;

  /// @copydoc MessageBufferInterface::reset()
  bool reset() override;

  /// @copydoc MessageBufferInterface::isEmpty()
  bool isEmpty() override;

  /// @copydoc MessageBufferInterface::isFull()
  bool isFull() override;

  /// @copydoc MessageBufferInterface::numMessagesAvailable()
  size_t numMessagesAvailable() override;

  /// @copydoc MessageBufferInterface::numBytesAvailable()
  size_t numBytesAvailable() override;

  /**
   * @copydoc OsExtensionInterface::getHandleForQueueSet
   */
  OsExtensionHandleType getHandleForQueueSet() const override;

protected:
  /**
   * @brief Get the internal FreeRTOS message buffer handle
   *
   * @return FreeRTOS queue handle
   */
  MessageBufferHandle_t getHandle() const {
    return _handle;
  }

  /**
   * @brief Set the internal FreeRTOS message buffer handle
   *
   * @param[in] handle handle
   */
  void setHandle(MessageBufferHandle_t handle) {
    _handle = handle;
  }

  /**
   * @brief Set the internal counting semaphore reference
   *
   * @param[in] countingSemaphoreReference counting semaphore reference
   */
  void setCountingSemaphoreReference(SemaphoreReference countingSemaphoreReference) {
    _countingSemaphoreReference = std::move(countingSemaphoreReference);
  }

  /**
   * @brief Set the internal binary semaphore reference
   *
   * @param[in] binarySemaphoreReference binary semaphore reference
   */
  void setBinarySemaphoreReference(SemaphoreReference binarySemaphoreReference) {
    _binarySemaphoreReference = std::move(binarySemaphoreReference);
  }

  /**
   * @brief Set the length
   *
   * @param[in] length length
   */
  void setLength(uint32_t length) {
    _length = length;
  }

private:
  /// represents FreeRTOS binary semaphore handle
  SemaphoreReference _binarySemaphoreReference;

  /// represents FreeRTOS counting semaphore handle
  SemaphoreReference _countingSemaphoreReference;

  /// Internal FreeRTOS handle
  MessageBufferHandle_t _handle{nullptr};

  /// Length
  uint32_t _length = 0u;
};

/**
 * @brief FreeRTOS Message Buffer with external storage buffer
 *
 * This class is encapsulates a FreeRTOS message buffer where the storage buffer
 * for the FreeRTOS queue is outside of the object.
 *
 * @tparam size Size (number of bytes) of message buffer
 * @tparam length Length (number of items) of message buffer
 */
template <std::size_t size, std::size_t length>
class MessageBufferWithExternalMemory : public MessageBufferReference {
public:
  /**
   * @brief Constructs message buffer with external memory
   *
   * @param[in] storageBuffer Reference to storage buffer of the message buffer
   */
  explicit MessageBufferWithExternalMemory(std::array<uint8_t, size>& storageBuffer) : _staticBinarySemaphore{1u, 1u}, _staticCountingSemaphore{length, 0u} {
    auto* const handle = osabstraction::messageBufferCreateStatic(size, reinterpret_cast<uint8_t*>(&storageBuffer), &_buffer);  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast) reinterpret_cast required due to external API
    this->setHandle(handle);

    this->setCountingSemaphoreReference(_staticCountingSemaphore.getReference());
    this->setBinarySemaphoreReference(_staticBinarySemaphore.getReference());
    this->setLength(length);
  }

  /// Deleted copy constructor
  MessageBufferWithExternalMemory(MessageBufferWithExternalMemory const&) = delete;

  /// Deleted move constructor
  MessageBufferWithExternalMemory(MessageBufferWithExternalMemory&&) = delete;

  /// Delete copy assignment operator @return
  MessageBufferWithExternalMemory& operator=(MessageBufferWithExternalMemory const&) = delete;

  /// Deleted move assignment operator @return
  MessageBufferWithExternalMemory& operator=(MessageBufferWithExternalMemory&&) = delete;

  /// Destructor
  ~MessageBufferWithExternalMemory() override {
    osabstraction::messageBufferDelete(this->getHandle());
  }

  /**
   * @brief Gets a reference to this message buffer
   * @return Reference to this message buffer
   */
  MessageBufferReference getReference() const {
    return MessageBufferReference(*this);  //NOLINT(cppcoreguidelines-slicing) this is exactly what we want here
  }

private:
  /// Static message buffer
  StaticMessageBuffer_t _buffer{};

  /// represents FreeRTOS binary semaphore object
  Semaphore _staticBinarySemaphore;

  /// represents FreeRTOS counting semaphore object
  Semaphore _staticCountingSemaphore;
};

/**
 * @brief FreeRTOS Message Buffer with internal storage buffer
 *
 * This class is encapsulates a FreeRTOS message buffer where the storage buffer
 * for the FreeRTOS queue is outside of the object.
 *
 * @tparam size Size (number of bytes) of message buffer
 * @tparam length Length (number of items) of message buffer
 */
template <std::size_t size, std::size_t length>
class MessageBuffer : public MessageBufferWithExternalMemory<size, length> {
public:
  /**
   * @brief: Constructs message buffer
   */
  MessageBuffer() : MessageBufferWithExternalMemory<size, length>{_storageBuffer} {}

  /// Deleted copy constructor
  MessageBuffer(MessageBuffer const&) = delete;

  /// Deleted move constructor
  MessageBuffer(MessageBuffer&&) = delete;

  /// Delete copy assignment operator @return
  MessageBuffer& operator=(MessageBuffer const&) = delete;

  /// Deleted move assignment operator @return
  MessageBuffer& operator=(MessageBuffer&&) = delete;

  /// Destructor
  ~MessageBuffer() override = default;

private:
  /// Storage buffer
  std::array<uint8_t, size> _storageBuffer{};
};

}  // namespace osextension

#endif /* OSEXTENSION_MESSAGEBUFFER_HPP */
