/**
 * @file src/OsAbstraction/OsAdapter/FreeRtosTestAdapter/FreeRtosTestAdapter.cpp
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

#include <FreeRtosMock.hpp>

namespace osabstraction {

extern "C" {

/// @copydoc queueCreateSetStatic()
QueueSetHandle_t queueCreateSetStatic(const UBaseType_t uxEventQueueLength, uint8_t* pucQueueSetStorage, StaticQueue_t* pxStaticQueueSet) {
  QueueSetHandle_t pxQueue = queueGenericCreateStatic(uxEventQueueLength, static_cast<UBaseType_t>(sizeof(void*)), pucQueueSetStorage,
                                                      pxStaticQueueSet, queueQUEUE_TYPE_SET);

  return pxQueue;
}

/* task.h abstraction */

/// @copydoc taskDelay()
void taskDelay(const TickType_t xTicksToDelay) {
  return freertos::mock->vTaskDelay(xTicksToDelay);
}

/// @copydoc taskGetName()
char* taskGetName(TaskHandle_t xTaskToQuery) {
  return freertos::mock->pcTaskGetName(xTaskToQuery);
}

/// @copydoc taskGetTickCount()
TickType_t taskGetTickCount(void) {
  return freertos::mock->xTaskGetTickCount();
}

/// @copydoc taskGetCurrentTaskHandle()
TaskHandle_t taskGetCurrentTaskHandle(void) {
  return freertos::mock->xTaskGetCurrentTaskHandle();
}

/// @copydoc taskGetTickCountFromISR()
TickType_t taskGetTickCountFromISR(void) {
  return freertos::mock->xTaskGetTickCountFromISR();
}

/// @copydoc taskGetSchedulerState()
BaseType_t taskGetSchedulerState(void) {
  return freertos::mock->xTaskGetSchedulerState();
}

/// @copydoc taskSuspend()
void taskSuspend(TaskHandle_t xTaskToSuspend) {
  freertos::mock->vTaskSuspend(xTaskToSuspend);
}

/* queue.h abstraction QueueSets */

/// @copydoc queueSelectFromSet()
QueueSetMemberHandle_t queueSelectFromSet(QueueSetHandle_t xQueueSet, TickType_t xBlockTimeTicks) {
  return freertos::mock->xQueueSelectFromSet(xQueueSet, xBlockTimeTicks);
}

/// @copydoc queueAddToSet()
BaseType_t queueAddToSet(QueueSetMemberHandle_t xQueueOrSemaphore, QueueSetHandle_t xQueueSet) {
  return freertos::mock->xQueueAddToSet(xQueueOrSemaphore, xQueueSet);
}

/// @copydoc queueRemoveFromSet()
BaseType_t queueRemoveFromSet(QueueSetMemberHandle_t xQueueOrSemaphore, QueueSetHandle_t xQueueSet) {
  return freertos::mock->xQueueRemoveFromSet(xQueueOrSemaphore, xQueueSet);
}

/* queue.h abstraction */

/// @copydoc queueGenericCreateStatic()
QueueHandle_t queueGenericCreateStatic(const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize, uint8_t* pucQueueStorage,
                                       StaticQueue_t* pxStaticQueue, const uint8_t ucQueueType) {
  return freertos::mock->xQueueGenericCreateStatic(uxQueueLength, uxItemSize, pucQueueStorage, pxStaticQueue, ucQueueType);
}

/// @copydoc queueCreateStatic()
QueueHandle_t queueCreateStatic(UBaseType_t uxQueueLength, UBaseType_t uxItemSize, uint8_t* pucQueueStorage, StaticQueue_t* pxQueueBuffer) {
  return freertos::mock->xQueueGenericCreateStatic((uxQueueLength), (uxItemSize), (pucQueueStorage), (pxQueueBuffer), (queueQUEUE_TYPE_BASE));
}

/// @copydoc queueRegister()
void queueRegister(QueueHandle_t xQueue, const char* name) {
#if (configQUEUE_REGISTRY_SIZE > 0)
  freertos::mock->vQueueAddToRegistry(xQueue, name);
#endif
}

/// @copydoc queueUnregister()
void queueUnregister(QueueHandle_t xQueue) {
#if (configQUEUE_REGISTRY_SIZE > 0)
  freertos::mock->vQueueUnregisterQueue(xQueue);
#endif
}

/// @copydoc queueGenericSend()
BaseType_t queueGenericSend(QueueHandle_t xQueue, const void* const pvItemToQueue, TickType_t xTicksToWait, const BaseType_t xCopyPosition) {
  return freertos::mock->xQueueGenericSend(xQueue, pvItemToQueue, xTicksToWait, xCopyPosition);
}

/// @copydoc queueSend()
BaseType_t queueSend(QueueHandle_t xQueue, const void* pvItemToQueue, TickType_t xTicksToWait) {
  return freertos::mock->xQueueGenericSend((xQueue), (pvItemToQueue), (xTicksToWait), queueSEND_TO_BACK);
}

/// @copydoc queueGenericSendFromISR()
BaseType_t queueGenericSendFromISR(QueueHandle_t xQueue, const void* const pvItemToQueue, BaseType_t* const pxHigherPriorityTaskWoken, const BaseType_t xCopyPosition) {
  return freertos::mock->xQueueGenericSendFromISR(xQueue, pvItemToQueue, pxHigherPriorityTaskWoken, xCopyPosition);
}

/// @copydoc queueSendFromISR()
BaseType_t queueSendFromISR(QueueHandle_t xQueue, const void* pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken) {
  return freertos::mock->xQueueGenericSendFromISR((xQueue), (pvItemToQueue), (pxHigherPriorityTaskWoken), queueSEND_TO_BACK);
}

/// @copydoc queueReceive()
BaseType_t queueReceive(QueueHandle_t xQueue, void* pvBuffer, TickType_t xTicksToWait) {
  return freertos::mock->xQueueReceive(xQueue, pvBuffer, xTicksToWait);
}

/// @copydoc queueReceiveFromISR()
BaseType_t queueReceiveFromISR(QueueHandle_t xQueue, void* pvBuffer, BaseType_t* pxHigherPriorityTaskWoken) {
  return freertos::mock->xQueueReceiveFromISR(xQueue, pvBuffer, pxHigherPriorityTaskWoken);
}

/// @copydoc queueGenericReset()
BaseType_t queueGenericReset(QueueHandle_t xQueue, BaseType_t xNewQueue) {
  return freertos::mock->xQueueGenericReset(xQueue, xNewQueue);
}

/// @copydoc queueReset()
BaseType_t queueReset(QueueHandle_t xQueue) {
  return freertos::mock->xQueueGenericReset(xQueue, pdFALSE);
}

/// @copydoc queueSpacesAvailable()
UBaseType_t queueSpacesAvailable(QueueHandle_t xQueue) {
  return freertos::mock->uxQueueSpacesAvailable(xQueue);
}

/// @copydoc queueDelete()
void queueDelete(QueueHandle_t xQueue) {
  freertos::mock->vQueueDelete(xQueue);
}

/* stream_buffer.h abstraction */

/// @copydoc streamBufferGenericCreateStatic()
StreamBufferHandle_t streamBufferGenericCreateStatic(size_t xBufferSizeBytes, size_t xTriggerLevelBytes, BaseType_t xIsMessageBuffer,
                                                     uint8_t* const pucStreamBufferStorageArea,
                                                     StaticStreamBuffer_t* const pxStaticStreamBuffer) {
  return freertos::mock->xStreamBufferGenericCreateStatic(xBufferSizeBytes, xTriggerLevelBytes, xIsMessageBuffer, pucStreamBufferStorageArea, pxStaticStreamBuffer);
}

/// @copydoc streamBufferSend()
size_t streamBufferSend(StreamBufferHandle_t xStreamBuffer, const void* pvTxData, size_t xDataLengthBytes, TickType_t xTicksToWait) {
  return freertos::mock->xStreamBufferSend(xStreamBuffer, pvTxData, xDataLengthBytes, xTicksToWait);
}

/// @copydoc streamBufferSendFromISR()
size_t streamBufferSendFromISR(StreamBufferHandle_t xStreamBuffer,
                               const void* pvTxData,
                               size_t xDataLengthBytes,
                               BaseType_t* const pxHigherPriorityTaskWoken) {
  return freertos::mock->xStreamBufferSendFromISR(xStreamBuffer, pvTxData, xDataLengthBytes, pxHigherPriorityTaskWoken);
}

/// @copydoc streamBufferReceive()
size_t streamBufferReceive(StreamBufferHandle_t xStreamBuffer, void* pvRxData, size_t xBufferLengthBytes, TickType_t xTicksToWait) {
  return freertos::mock->xStreamBufferReceive(xStreamBuffer, pvRxData, xBufferLengthBytes, xTicksToWait);
}

/// @copydoc streamBufferReceiveFromISR()
size_t streamBufferReceiveFromISR(StreamBufferHandle_t xStreamBuffer,
                                  void* pvRxData,
                                  size_t xBufferLengthBytes,
                                  BaseType_t* const pxHigherPriorityTaskWoken) {
  return freertos::mock->xStreamBufferReceiveFromISR(xStreamBuffer, pvRxData, xBufferLengthBytes, pxHigherPriorityTaskWoken);
}

/// @copydoc streamBufferDelete()
void streamBufferDelete(StreamBufferHandle_t xStreamBuffer) {
  freertos::mock->vStreamBufferDelete(xStreamBuffer);
}

/// @copydoc streamBufferBytesAvailable()
size_t streamBufferBytesAvailable(StreamBufferHandle_t xStreamBuffer) {
  return freertos::mock->xStreamBufferBytesAvailable(xStreamBuffer);
}

/// @copydoc streamBufferSpacesAvailable()
size_t streamBufferSpacesAvailable(StreamBufferHandle_t xStreamBuffer) {
  return freertos::mock->xStreamBufferSpacesAvailable(xStreamBuffer);
}

/// @copydoc streamBufferReset()
BaseType_t streamBufferReset(StreamBufferHandle_t xStreamBuffer) {
  return freertos::mock->xStreamBufferReset(xStreamBuffer);
}

/// @copydoc streamBufferIsEmpty()
BaseType_t streamBufferIsEmpty(StreamBufferHandle_t xStreamBuffer) {
  return freertos::mock->xStreamBufferIsEmpty(xStreamBuffer);
}

/// @copydoc streamBufferIsFull()
BaseType_t streamBufferIsFull(StreamBufferHandle_t xStreamBuffer) {
  return freertos::mock->xStreamBufferIsFull(xStreamBuffer);
}

/* message_buffer.h abstraction */

/// @copydoc messageBufferCreateStatic()
MessageBufferHandle_t messageBufferCreateStatic(size_t xBufferSizeBytes, uint8_t* pucMessageBufferStorageArea,
                                                StaticMessageBuffer_t* pxStaticMessageBuffer) {
  return static_cast<MessageBufferHandle_t>(streamBufferGenericCreateStatic(xBufferSizeBytes, 0, pdTRUE, pucMessageBufferStorageArea, pxStaticMessageBuffer));
}

/// @copydoc messageBufferSend()
size_t messageBufferSend(MessageBufferHandle_t xMessageBuffer,
                         const void* pvTxData,
                         size_t xDataLengthBytes,
                         TickType_t xTicksToWait) {
  return streamBufferSend(static_cast<StreamBufferHandle_t>(xMessageBuffer), pvTxData, xDataLengthBytes, xTicksToWait);
}

/// @copydoc messageBufferSendFromISR()
size_t messageBufferSendFromISR(MessageBufferHandle_t xMessageBuffer,
                                const void* pvTxData,
                                size_t xDataLengthBytes,
                                BaseType_t* pxHigherPriorityTaskWoken) {
  return streamBufferSendFromISR(static_cast<StreamBufferHandle_t>(xMessageBuffer), pvTxData, xDataLengthBytes, pxHigherPriorityTaskWoken);
}

/// @copydoc messageBufferReceive()
size_t messageBufferReceive(MessageBufferHandle_t xMessageBuffer,
                            void* pvRxData,
                            size_t xBufferLengthBytes,
                            TickType_t xTicksToWait) {
  return streamBufferReceive(static_cast<StreamBufferHandle_t>(xMessageBuffer), pvRxData, xBufferLengthBytes, xTicksToWait);
}

/// @copydoc messageBufferReceiveFromISR()
size_t messageBufferReceiveFromISR(MessageBufferHandle_t xMessageBuffer,
                                   void* pvRxData,
                                   size_t xBufferLengthBytes,
                                   BaseType_t* pxHigherPriorityTaskWoken) {
  return streamBufferReceiveFromISR(static_cast<StreamBufferHandle_t>(xMessageBuffer), pvRxData, xBufferLengthBytes, pxHigherPriorityTaskWoken);
}

/// @copydoc messageBufferDelete()
void messageBufferDelete(MessageBufferHandle_t xMessageBuffer) {
  streamBufferDelete(static_cast<StreamBufferHandle_t>(xMessageBuffer));
}

/// @copydoc messageBufferSpacesAvailable()
size_t messageBufferSpacesAvailable(MessageBufferHandle_t xMessageBuffer) {
  return streamBufferSpacesAvailable(static_cast<StreamBufferHandle_t>(xMessageBuffer));
}

/// @copydoc messageBufferReset()
BaseType_t messageBufferReset(MessageBufferHandle_t xMessageBuffer) {
  return streamBufferReset(static_cast<StreamBufferHandle_t>(xMessageBuffer));
}

/// @copydoc messageBufferIsEmpty()
BaseType_t messageBufferIsEmpty(MessageBufferHandle_t xMessageBuffer) {
  return streamBufferIsEmpty(static_cast<StreamBufferHandle_t>(xMessageBuffer));
}

/// @copydoc messageBufferIsFull()
BaseType_t messageBufferIsFull(MessageBufferHandle_t xMessageBuffer) {
  return streamBufferIsFull(static_cast<StreamBufferHandle_t>(xMessageBuffer));
}

/* semphr.h abstraction */

/// @copydoc semaphoreCreateCountingStatic()
SemaphoreHandle_t semaphoreCreateCountingStatic(UBaseType_t uxMaxCount, UBaseType_t uxInitialCount, StaticSemaphore_t* pxSemaphoreBuffer) {
  return freertos::mock->xQueueCreateCountingSemaphoreStatic((uxMaxCount), (uxInitialCount), (pxSemaphoreBuffer));
}

/// @copydoc semaphoreCreateBinaryStatic()
SemaphoreHandle_t semaphoreCreateBinaryStatic(StaticSemaphore_t* pxSemaphoreBuffer) {
  return freertos::mock->xSemaphoreCreateBinaryStatic(pxSemaphoreBuffer);
}

/// @copydoc mutexCreateStatic()
SemaphoreHandle_t mutexCreateStatic(StaticSemaphore_t* pxMutexBuffer) {
  return freertos::mock->xSemaphoreCreateMutexStatic(pxMutexBuffer);
}

/// @copydoc semaphoreDelete()
void semaphoreDelete(SemaphoreHandle_t xSemaphore) {
  queueDelete(static_cast<QueueHandle_t>(xSemaphore));
}

/// @copydoc semaphoreTake()
BaseType_t semaphoreTake(SemaphoreHandle_t xSemaphore, TickType_t xBlockTime) {
  return freertos::mock->xQueueSemaphoreTake((xSemaphore), (xBlockTime));
}

/// @copydoc semaphoreTakeFromISR()
BaseType_t semaphoreTakeFromISR(SemaphoreHandle_t xSemaphore, BaseType_t* pxHigherPriorityTaskWoken) {
  return queueReceiveFromISR(static_cast<QueueHandle_t>(xSemaphore), nullptr, (pxHigherPriorityTaskWoken));
}

/// @copydoc semaphoreGive()
BaseType_t semaphoreGive(SemaphoreHandle_t xSemaphore) {
  return queueGenericSend(static_cast<QueueHandle_t>(xSemaphore), nullptr, semGIVE_BLOCK_TIME, queueSEND_TO_BACK);
}

/// @copydoc semaphoreGiveFromISR()
BaseType_t semaphoreGiveFromISR(SemaphoreHandle_t xSemaphore, BaseType_t* pxHigherPriorityTaskWoken) {
  return freertos::mock->xQueueGiveFromISR(static_cast<QueueHandle_t>(xSemaphore), (pxHigherPriorityTaskWoken));
}

/// @copydoc semaphoreGetCount()
UBaseType_t semaphoreGetCount(SemaphoreHandle_t xSemaphore) {
  return freertos::mock->uxQueueMessagesWaiting(xSemaphore);
}

/// @copydoc semaphoreGetCountFromISR()
UBaseType_t semaphoreGetCountFromISR(SemaphoreHandle_t xSemaphore) {
  return freertos::mock->uxQueueMessagesWaitingFromISR(static_cast<QueueHandle_t>(xSemaphore));
}

/* portmacro.h abstraction */

/// @copydoc yieldFromISR()
void yieldFromISR(const BaseType_t xHigherPriorityTaskWoken) {
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

}  // extern "C"

}  // namespace osabstraction
