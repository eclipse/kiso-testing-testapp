/**
 * @file src/OsExtension/MessageBuffer.cpp
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

#include <OsExtension/MessageBuffer.hpp>

namespace osextension {

size_t MessageBufferReference::send(const void* data, size_t length) {
  return send(data, length, 0u);
}

size_t MessageBufferReference::send(const void* data, size_t length, TickType_t ticksToWait) {
  size_t result = 0u;
  if (this->_binarySemaphoreReference.wait(portMAX_DELAY)) {
    // do only allow to push to message buffer when counting semaphore can be signalled
    if (_countingSemaphoreReference.getCount() < _length) {
      result = osabstraction::messageBufferSend(_handle, data, length, ticksToWait);

      if (result != 0u) {
        notifyObserver(DataAddedEvent{*this, data, length});
        this->_countingSemaphoreReference.signal();
      } else {
        notifyObserver(DataDroppedEvent{*this, data, length});
      }
    }
    this->_binarySemaphoreReference.signal();
  }
  return result;
}

size_t MessageBufferReference::send(etl::array_view<const uint8_t> data) {
  return send(&data[0], data.size(), 0u);
}

size_t MessageBufferReference::send(etl::array_view<const uint8_t> data, TickType_t ticksToWait) {
  return send(&data[0], data.size(), ticksToWait);
}

size_t MessageBufferReference::sendFromISR(const void* data, size_t length, BaseType_t* higherPriorityTaskWoken) {
  size_t result = 0u;
  if (this->_binarySemaphoreReference.waitFromIsr(higherPriorityTaskWoken)) {
    // do only allow to push to message buffer when counting semaphore can be signalled
    if (_countingSemaphoreReference.getCountFromIsr() < _length) {
      result = osabstraction::messageBufferSendFromISR(_handle, data, length, higherPriorityTaskWoken);

      if (result != 0u) {
        notifyObserver(DataAddedEvent{*this, data, length});
        this->_countingSemaphoreReference.signalFromIsr(higherPriorityTaskWoken);
      } else {
        notifyObserver(DataDroppedEvent{*this, data, length});
      }
    }
    this->_binarySemaphoreReference.signalFromIsr(higherPriorityTaskWoken);
  }
  return result;
}

size_t MessageBufferReference::sendFromISR(etl::array_view<const uint8_t> data, BaseType_t* higherPriorityTaskWoken) {
  return sendFromISR(&data[0], data.size(), higherPriorityTaskWoken);
}

size_t MessageBufferReference::receive(void* data, size_t length, TickType_t xTicksToWait) {
  size_t result = 0u;
  if (this->_countingSemaphoreReference.wait(xTicksToWait)) {
    result = osabstraction::messageBufferReceive(_handle, data, length, 0u);
    if (result != 0u) {
      notifyObserver(DataRemovedEvent{*this, data, length});
    }
  }
  return result;
}

size_t MessageBufferReference::receive(etl::array_view<uint8_t> data, TickType_t xTicksToWait) {
  return receive(&data[0], data.size(), xTicksToWait);
}

size_t MessageBufferReference::receiveFromISR(void* data, size_t length, BaseType_t* higherPriorityTaskWoken) {
  size_t result = 0u;
  if (this->_countingSemaphoreReference.waitFromIsr(higherPriorityTaskWoken)) {
    result = osabstraction::messageBufferReceiveFromISR(_handle, data, length, higherPriorityTaskWoken);
    if (result != 0u) {
      notifyObserver(DataRemovedEvent{*this, data, length});
    }
  }
  return result;
}

size_t MessageBufferReference::receiveFromISR(etl::array_view<uint8_t> data, BaseType_t* higherPriorityTaskWoken) {
  return receiveFromISR(&data[0], data.size(), higherPriorityTaskWoken);
}

bool MessageBufferReference::reset() {
  this->_countingSemaphoreReference.reset();
  return osabstraction::messageBufferReset(_handle) == pdTRUE;
}

bool MessageBufferReference::isEmpty() {
  return osabstraction::messageBufferIsEmpty(_handle) == pdTRUE;
}

bool MessageBufferReference::isFull() {
  return osabstraction::messageBufferIsFull(_handle) == pdTRUE;
}

size_t MessageBufferReference::numMessagesAvailable() {
  return _countingSemaphoreReference.getCount();
}

size_t MessageBufferReference::numBytesAvailable() {
  return osabstraction::streamBufferBytesAvailable(static_cast<StreamBufferHandle_t>(_handle));
}

OsExtensionInterface::OsExtensionHandleType MessageBufferReference::getHandleForQueueSet() const {
  return this->_countingSemaphoreReference.getHandleForQueueSet();
}

}  // namespace osextension
