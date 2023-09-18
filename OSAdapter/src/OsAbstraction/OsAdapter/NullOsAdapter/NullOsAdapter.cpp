/**
 * @file src/OsAbstraction/OsAdapter/NullOsAdapter/NullOsAdapter.cpp
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 */
#include <OsAbstraction/OsAbstraction.hpp>

namespace osabstraction {

QueueSetHandle_t queueCreateSetStatic(const UBaseType_t uxEventQueueLength, uint8_t* pucQueueSetStorage, StaticQueue_t* pxStaticQueueSet) {
  QueueSetHandle_t pxQueue = queueGenericCreateStatic(uxEventQueueLength, static_cast<UBaseType_t>(sizeof(void*)), pucQueueSetStorage,
                                                      pxStaticQueueSet, queueQUEUE_TYPE_SET);

  return pxQueue;
}

/* task.h abstraction */

void taskDelay(const TickType_t xTicksToDelay __attribute__((unused))) {
}

char* taskGetName(TaskHandle_t xTaskToQuery __attribute__((unused))) {
  return nullptr;
}

TaskHandle_t taskGetCurrentTaskHandle(void) {
  return nullptr;
}

TickType_t taskGetTickCount(void) {
  return 0u;
}

TickType_t taskGetTickCountFromISR(void) {
  return 0u;
}

BaseType_t taskGetSchedulerState(void) {
  // scheduler not started is appropriate value: No OS --> no scheduler
  // value for 'scheduler not stated' adopted from FreeRTOS task.h
  return taskSCHEDULER_NOT_STARTED;
}

void taskSuspend(TaskHandle_t xTaskToSuspend __attribute__((unused))) {
}

/* queue.h abstraction QueueSets */

QueueSetMemberHandle_t queueSelectFromSet(QueueSetHandle_t xQueueSet __attribute__((unused)),
                                          TickType_t xBlockTimeTicks __attribute__((unused))) {
  return nullptr;
}

BaseType_t queueAddToSet(QueueSetMemberHandle_t xQueueOrSemaphore __attribute__((unused)),
                         QueueSetHandle_t xQueueSet __attribute__((unused))) {
  return 0u;
}

BaseType_t queueRemoveFromSet(QueueSetMemberHandle_t xQueueOrSemaphore __attribute__((unused)),
                              QueueSetHandle_t xQueueSet __attribute__((unused))) {
  return 0u;
}

/* queue.h abstraction */

QueueHandle_t queueGenericCreateStatic(const UBaseType_t uxQueueLength __attribute__((unused)),
                                       const UBaseType_t uxItemSize __attribute__((unused)),
                                       uint8_t* pucQueueStorage __attribute__((unused)),
                                       StaticQueue_t* pxStaticQueue __attribute__((unused)),
                                       const uint8_t ucQueueType __attribute__((unused))) {
  return nullptr;
}

QueueHandle_t queueCreateStatic(UBaseType_t uxQueueLength __attribute__((unused)), UBaseType_t uxItemSize __attribute__((unused)), uint8_t* pucQueueStorage __attribute__((unused)), StaticQueue_t* pxQueueBuffer __attribute__((unused))) {
  return nullptr;
}

void queueRegister(QueueHandle_t xQueue __attribute__((unused)), const char* name __attribute__((unused))) {
}

void queueUnregister(QueueHandle_t xQueue __attribute__((unused))) {
}

BaseType_t queueGenericSend(QueueHandle_t xQueue __attribute__((unused)),
                            const void* const pvItemToQueue __attribute__((unused)),
                            TickType_t xTicksToWait __attribute__((unused)),
                            const BaseType_t xCopyPosition __attribute__((unused))) {
  return 0u;
}

BaseType_t queueSend(QueueHandle_t xQueue __attribute__((unused)),
                     const void* pvItemToQueue __attribute__((unused)),
                     TickType_t xTicksToWait __attribute__((unused))) {
  return 0u;
}

BaseType_t queueGenericSendFromISR(QueueHandle_t xQueue __attribute__((unused)),
                                   const void* const pvItemToQueue __attribute__((unused)),
                                   BaseType_t* const pxHigherPriorityTaskWoken __attribute__((unused)),
                                   const BaseType_t xCopyPosition __attribute__((unused))) {
  return 0u;
}

BaseType_t queueSendFromISR(QueueHandle_t xQueue __attribute__((unused)),
                            const void* pvItemToQueue __attribute__((unused)),
                            BaseType_t* pxHigherPriorityTaskWoken __attribute__((unused))) {
  return 0u;
}

BaseType_t queueReceive(QueueHandle_t xQueue __attribute__((unused)),
                        void* pvBuffer __attribute__((unused)),
                        TickType_t xTicksToWait __attribute__((unused))) {
  return 0u;
}

BaseType_t queueReceiveFromISR(QueueHandle_t xQueue __attribute__((unused)),
                               void* pvBuffer __attribute__((unused)),
                               BaseType_t* pxHigherPriorityTaskWoken __attribute__((unused))) {
  return 0u;
}

BaseType_t queueGenericReset(QueueHandle_t xQueue __attribute__((unused)),
                             BaseType_t xNewQueue __attribute__((unused))) {
  return 0u;
}

BaseType_t queueReset(QueueHandle_t xQueue __attribute__((unused))) {
  return 0u;
}

UBaseType_t queueSpacesAvailable(QueueHandle_t xQueue __attribute__((unused))) {
  return 0u;
}

void queueDelete(QueueHandle_t xQueue __attribute__((unused))) {
}

/* stream_buffer.h abstraction */

StreamBufferHandle_t streamBufferGenericCreateStatic(size_t xBufferSizeBytes __attribute__((unused)),
                                                     size_t xTriggerLevelBytes __attribute__((unused)),
                                                     BaseType_t xIsMessageBuffer __attribute__((unused)),
                                                     uint8_t* const pucStreamBufferStorageArea __attribute__((unused)),
                                                     StaticStreamBuffer_t* const pxStaticStreamBuffer __attribute__((unused))) {
  return nullptr;
}

size_t streamBufferSend(StreamBufferHandle_t xStreamBuffer __attribute__((unused)),
                        const void* pvTxData __attribute__((unused)),
                        size_t xDataLengthBytes __attribute__((unused)),
                        TickType_t xTicksToWait __attribute__((unused))) {
  return 0u;
}

size_t streamBufferSendFromISR(StreamBufferHandle_t xStreamBuffer __attribute__((unused)),
                               const void* pvTxData __attribute__((unused)),
                               size_t xDataLengthBytes __attribute__((unused)),
                               BaseType_t* const pxHigherPriorityTaskWoken __attribute__((unused))) {
  return 0u;
}

size_t streamBufferReceive(StreamBufferHandle_t xStreamBuffer __attribute__((unused)),
                           void* pvRxData __attribute__((unused)),
                           size_t xBufferLengthBytes __attribute__((unused)),
                           TickType_t xTicksToWait __attribute__((unused))) {
  return 0u;
}

size_t streamBufferReceiveFromISR(StreamBufferHandle_t xStreamBuffer __attribute__((unused)),
                                  void* pvRxData __attribute__((unused)),
                                  size_t xBufferLengthBytes __attribute__((unused)),
                                  BaseType_t* const pxHigherPriorityTaskWoken __attribute__((unused))) {
  return 0u;
}

void streamBufferDelete(StreamBufferHandle_t xStreamBuffer __attribute__((unused))) {
}

size_t streamBufferBytesAvailable(StreamBufferHandle_t xStreamBuffer __attribute__((unused))) {
  return 0u;
}

size_t streamBufferSpacesAvailable(StreamBufferHandle_t xStreamBuffer __attribute__((unused))) {
  return 0u;
}

BaseType_t streamBufferReset(StreamBufferHandle_t xStreamBuffer __attribute__((unused))) {
  return 0u;
}

BaseType_t streamBufferIsEmpty(StreamBufferHandle_t xStreamBuffer __attribute__((unused))) {
  return 0u;
}

BaseType_t streamBufferIsFull(StreamBufferHandle_t xStreamBuffer __attribute__((unused))) {
  return 0u;
}

/* message_buffer.h abstraction */

MessageBufferHandle_t messageBufferCreateStatic(size_t xBufferSizeBytes __attribute__((unused)),
                                                uint8_t* pucMessageBufferStorageArea __attribute__((unused)),
                                                StaticMessageBuffer_t* pxStaticMessageBuffer __attribute__((unused))) {
  return nullptr;
}

size_t messageBufferSend(MessageBufferHandle_t xMessageBuffer __attribute__((unused)),
                         const void* pvTxData __attribute__((unused)),
                         size_t xDataLengthBytes __attribute__((unused)),
                         TickType_t xTicksToWait __attribute__((unused))) {
  return 0u;
}

size_t messageBufferSendFromISR(MessageBufferHandle_t xMessageBuffer __attribute__((unused)),
                                const void* pvTxData __attribute__((unused)),
                                size_t xDataLengthBytes __attribute__((unused)),
                                BaseType_t* pxHigherPriorityTaskWoken __attribute__((unused))) {
  return 0u;
}

size_t messageBufferReceive(MessageBufferHandle_t xMessageBuffer __attribute__((unused)),
                            void* pvRxData __attribute__((unused)),
                            size_t xBufferLengthBytes __attribute__((unused)),
                            TickType_t xTicksToWait __attribute__((unused))) {
  return 0u;
}

size_t messageBufferReceiveFromISR(MessageBufferHandle_t xMessageBuffer __attribute__((unused)),
                                   void* pvRxData __attribute__((unused)),
                                   size_t xBufferLengthBytes __attribute__((unused)),
                                   BaseType_t* pxHigherPriorityTaskWoken __attribute__((unused))) {
  return 0u;
}

void messageBufferDelete(MessageBufferHandle_t xMessageBuffer __attribute__((unused))) {
}

size_t messageBufferSpacesAvailable(MessageBufferHandle_t xMessageBuffer) {
  return streamBufferSpacesAvailable(static_cast<StreamBufferHandle_t>(xMessageBuffer));
}

BaseType_t messageBufferReset(MessageBufferHandle_t xMessageBuffer) {
  return streamBufferReset(static_cast<StreamBufferHandle_t>(xMessageBuffer));
}

BaseType_t messageBufferIsEmpty(MessageBufferHandle_t xMessageBuffer __attribute__((unused))) {
  return 0u;
}

BaseType_t messageBufferIsFull(MessageBufferHandle_t xMessageBuffer __attribute__((unused))) {
  return 0u;
}

/* semphr.h abstraction */

SemaphoreHandle_t semaphoreCreateCountingStatic(UBaseType_t uxMaxCount __attribute__((unused)),
                                                UBaseType_t uxInitialCount __attribute__((unused)),
                                                StaticSemaphore_t* pxSemaphoreBuffer __attribute__((unused))) {
  return nullptr;
}

SemaphoreHandle_t semaphoreCreateBinaryStatic(StaticSemaphore_t* pxSemaphoreBuffer __attribute__((unused))) {
  return nullptr;
}

SemaphoreHandle_t mutexCreateStatic(StaticSemaphore_t* pxMutexBuffer __attribute__((unused))) {
  return nullptr;
}

void semaphoreDelete(SemaphoreHandle_t xSemaphore __attribute__((unused))) {
}

BaseType_t semaphoreTake(SemaphoreHandle_t xSemaphore __attribute__((unused)),
                         TickType_t xBlockTime __attribute__((unused))) {
  return 0u;
}

BaseType_t semaphoreTakeFromISR(SemaphoreHandle_t xSemaphore __attribute__((unused)),
                                BaseType_t* pxHigherPriorityTaskWoken __attribute__((unused))) {
  return 0u;
}

BaseType_t semaphoreGive(SemaphoreHandle_t xSemaphore __attribute__((unused))) {
  return 0u;
}

BaseType_t semaphoreGiveFromISR(SemaphoreHandle_t xSemaphore __attribute__((unused)),
                                BaseType_t* pxHigherPriorityTaskWoken __attribute__((unused))) {
  return 0u;
}

UBaseType_t semaphoreGetCount(SemaphoreHandle_t xSemaphore __attribute__((unused))) {
  return 0u;
}

UBaseType_t semaphoreGetCountFromISR(SemaphoreHandle_t xSemaphore __attribute__((unused))) {
  return 0u;
}

/* portmacro.h abstraction */

void yieldFromISR(const BaseType_t xHigherPriorityTaskWoken __attribute__((unused))) {
}

}  // namespace osabstraction
