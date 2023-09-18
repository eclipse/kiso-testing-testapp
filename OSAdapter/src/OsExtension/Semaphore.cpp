/**
 * @file src/OsExtension/Semaphore.cpp
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

#include <OsExtension/Semaphore.hpp>

namespace osextension {

bool SemaphoreReference::signal(void) {
  return (osabstraction::semaphoreGive(_semaphore) == pdTRUE);
}

bool SemaphoreReference::signalFromIsr(SemaphoreTaskWokenTokenType* taskWoken) {
  return (osabstraction::semaphoreGiveFromISR(_semaphore, taskWoken) == pdTRUE);
}

bool SemaphoreReference::wait(uint32_t timeout) {
  return (osabstraction::semaphoreTake(_semaphore, timeout) == pdTRUE);
}

bool SemaphoreReference::waitFromIsr(SemaphoreTaskWokenTokenType* taskWoken) {
  return (osabstraction::semaphoreTakeFromISR(_semaphore, taskWoken) == pdTRUE);
}

bool SemaphoreReference::reset(void) {
  return (osabstraction::queueReset(_semaphore) == pdTRUE);
}

size_t SemaphoreReference::getCount(void) {
  return osabstraction::semaphoreGetCount(_semaphore);
}

size_t SemaphoreReference::getCountFromIsr(void) {
  return osabstraction::semaphoreGetCountFromISR(_semaphore);
}

}  // namespace osextension
