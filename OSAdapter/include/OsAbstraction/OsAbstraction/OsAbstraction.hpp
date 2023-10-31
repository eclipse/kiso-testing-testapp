/**
 * @file include/OsAbstraction/OsAbstraction/OsAbstraction.hpp
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 */

#ifndef OSABSTRACTION_OSABSTRACTION_HPP
#define OSABSTRACTION_OSABSTRACTION_HPP

// the define below will be passed to the compiler by cmake
// dependend on selected OsAdapter
// (DISABLE_FREERTOS is always set when FreeRtosAdapter is NOT selected)
#ifdef DISABLE_FREERTOS
#include <OsAbstraction/OsAbstractionTypes.hpp>
#else
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <message_buffer.h>
#include <semphr.h>
#endif

#include <cstdint>

#ifdef __cplusplus
/// @brief OS Abstraction
namespace osabstraction {
extern "C" {
#endif  // __cplusplus

/* task.h abstraction */

/**
 * @brief See FreeRTOS task.h
 * @param xTicksToDelay Ticks the calling task should block
 */
void taskDelay(TickType_t xTicksToDelay);

/**
 * @brief See FreeRTOS task.h
 * @param xTaskToQuery Task to query
 * @return Task name
 */
char* taskGetName(TaskHandle_t xTaskToQuery);

/**
 * @brief See FreeRTOS task.h
 * @return current Task handle
 */
TaskHandle_t taskGetCurrentTaskHandle(void);

/**
 * @brief See FreeRTOS task.h
 * @return Tick count
 */
TickType_t taskGetTickCount(void);

/**
 * @brief See FreeRTOS task.h
 * @return Tick count
 */
TickType_t taskGetTickCountFromISR(void);

/**
 * @brief See FreeRTOS task.h
 * @return Scheduler state
 */
BaseType_t taskGetSchedulerState(void);

/**
 * @brief See FreeRTOS task.h
 * @param xTaskToSuspend Handle of the task that shall be suspended.
 */
void taskSuspend(TaskHandle_t xTaskToSuspend);

/* queue.h abstraction QueueSets */

/**
 * @brief Creates a FreeRTOS QueueSet with static memory
 *
 * For further details see FreeRTOS queue.h xQueueCreateSet()
 * Note: Works just like a regular QueueSet with dynamic memory allocation
 *
 * @param uxEventQueueLength max amount of QueueSetMemberHandle_t in that QueueSet
 * @param pucQueueSetStorage static storage area of that QueueSet (should have sizeof(void*) * uxEventQueueLength in bytes)
 * @param pxStaticQueueSet StaticQueue_t struct defined by FreeRTOS
 * @return QueueSetHandle_t to be used by client
 */
QueueSetHandle_t queueCreateSetStatic(UBaseType_t uxEventQueueLength, uint8_t* pucQueueSetStorage, StaticQueue_t* pxStaticQueueSet);

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueueSet
 * @param xBlockTimeTicks
 * @return
 */
QueueSetMemberHandle_t queueSelectFromSet(QueueSetHandle_t xQueueSet, TickType_t xBlockTimeTicks);

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueueOrSemaphore
 * @param xQueueSet
 * @return
 */
BaseType_t queueAddToSet(QueueSetMemberHandle_t xQueueOrSemaphore, QueueSetHandle_t xQueueSet);

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueueOrSemaphore
 * @param xQueueSet
 * @return
 */
BaseType_t queueRemoveFromSet(QueueSetMemberHandle_t xQueueOrSemaphore, QueueSetHandle_t xQueueSet);

/* queue.h abstraction */

/**
 * @brief See FreeRTOS queue.h
 *
 * @param uxQueueLength
 * @param uxItemSize
 * @param pucQueueStorage
 * @param pxStaticQueue
 * @param ucQueueType
 * @return
 */
QueueHandle_t queueGenericCreateStatic(const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize, uint8_t* pucQueueStorage,  //NOLINT(readability-avoid-const-params-in-decls)
                                       StaticQueue_t* pxStaticQueue, const uint8_t ucQueueType);                                 //NOLINT(readability-avoid-const-params-in-decls)
/**
 * @brief See FreeRTOS queue.h
 *
 * @param uxQueueLength
 * @param uxItemSize
 * @param pucQueueStorage
 * @param pxQueueBuffer
 * @return
 */
QueueHandle_t queueCreateStatic(UBaseType_t uxQueueLength, UBaseType_t uxItemSize, uint8_t* pucQueueStorage, StaticQueue_t* pxQueueBuffer);

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueue
 * @param name
 */
void queueRegister(QueueHandle_t xQueue, const char* name);

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueue queue to unregister
 */
void queueUnregister(QueueHandle_t xQueue);

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueue
 * @param pvItemToQueue
 * @param xTicksToWait
 * @param xCopyPosition
 * @return
 */
BaseType_t queueGenericSend(QueueHandle_t xQueue, const void* const pvItemToQueue, TickType_t xTicksToWait, const BaseType_t xCopyPosition);  //NOLINT(readability-avoid-const-params-in-decls)

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueue
 * @param pvItemToQueue
 * @param xTicksToWait
 * @return
 */
BaseType_t queueSend(QueueHandle_t xQueue, const void* pvItemToQueue, TickType_t xTicksToWait);

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueue
 * @param pvItemToQueue
 * @param pxHigherPriorityTaskWoken
 * @param xCopyPosition
 * @return
 */
BaseType_t queueGenericSendFromISR(QueueHandle_t xQueue, const void* const pvItemToQueue, BaseType_t* const pxHigherPriorityTaskWoken,  //NOLINT(readability-avoid-const-params-in-decls)
                                   const BaseType_t xCopyPosition);                                                                     //NOLINT(readability-avoid-const-params-in-decls)

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueue
 * @param pvItemToQueue
 * @param pxHigherPriorityTaskWoken
 * @return
 */
BaseType_t queueSendFromISR(QueueHandle_t xQueue, const void* pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken);

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueue
 * @param pvBuffer
 * @param xTicksToWait
 * @return
 */
BaseType_t queueReceive(QueueHandle_t xQueue, void* pvBuffer, TickType_t xTicksToWait);

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueue
 * @param pvBuffer
 * @param pxHigherPriorityTaskWoken
 * @return
 */
BaseType_t queueReceiveFromISR(QueueHandle_t xQueue, void* pvBuffer, BaseType_t* pxHigherPriorityTaskWoken);

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueue
 * @param xNewQueue
 * @return
 */
BaseType_t queueGenericReset(QueueHandle_t xQueue, BaseType_t xNewQueue);

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueue
 * @return
 */
BaseType_t queueReset(QueueHandle_t xQueue);

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueue
 * @return
 */
UBaseType_t queueSpacesAvailable(QueueHandle_t xQueue);

/**
 * @brief See FreeRTOS queue.h
 *
 * @param xQueue
 */
void queueDelete(QueueHandle_t xQueue);

/* stream_buffer.h abstraction */

/**
 * @brief See FreeRTOS stream_buffer.h
 *
 * @param xBufferSizeBytes
 * @param xTriggerLevelBytes
 * @param xIsMessageBuffer
 * @param pucStreamBufferStorageArea
 * @param pxStaticStreamBuffer
 * @return
 */
StreamBufferHandle_t streamBufferGenericCreateStatic(size_t xBufferSizeBytes, size_t xTriggerLevelBytes, BaseType_t xIsMessageBuffer,
                                                     uint8_t* const pucStreamBufferStorageArea,          //NOLINT(readability-avoid-const-params-in-decls)
                                                     StaticStreamBuffer_t* const pxStaticStreamBuffer);  //NOLINT(readability-avoid-const-params-in-decls)

/**
 * @brief See FreeRTOS stream_buffer.h
 *
 * @param xStreamBuffer
 * @param pvTxData
 * @param xDataLengthBytes
 * @param xTicksToWait
 * @return
 */
size_t streamBufferSend(StreamBufferHandle_t xStreamBuffer, const void* pvTxData, size_t xDataLengthBytes, TickType_t xTicksToWait);

/**
 * @brief See FreeRTOS stream_buffer.h
 *
 * @param xStreamBuffer
 * @param pvTxData
 * @param xDataLengthBytes
 * @param pxHigherPriorityTaskWoken
 * @return
 */
size_t streamBufferSendFromISR(StreamBufferHandle_t xStreamBuffer,
                               const void* pvTxData,
                               size_t xDataLengthBytes,
                               BaseType_t* const pxHigherPriorityTaskWoken);  //NOLINT(readability-avoid-const-params-in-decls)

/**
 * @brief See FreeRTOS stream_buffer.h
 *
 * @param xStreamBuffer
 * @param pvRxData
 * @param xBufferLengthBytes
 * @param xTicksToWait
 * @return
 */
size_t streamBufferReceive(StreamBufferHandle_t xStreamBuffer, void* pvRxData, size_t xBufferLengthBytes, TickType_t xTicksToWait);

/**
 * @brief See FreeRTOS stream_buffer.h
 *
 * @param xStreamBuffer
 * @param pvRxData
 * @param xBufferLengthBytes
 * @param pxHigherPriorityTaskWoken
 * @return
 */
size_t streamBufferReceiveFromISR(StreamBufferHandle_t xStreamBuffer,
                                  void* pvRxData,
                                  size_t xBufferLengthBytes,
                                  BaseType_t* const pxHigherPriorityTaskWoken);  //NOLINT(readability-avoid-const-params-in-decls)

/**
 * @brief See FreeRTOS stream_buffer.h
 *
 * @param xStreamBuffer
 */
void streamBufferDelete(StreamBufferHandle_t xStreamBuffer);

/**
 * @brief See FreeRTOS stream_buffer.h
 *
 * @param xStreamBuffer
 * @return
 */
size_t streamBufferBytesAvailable(StreamBufferHandle_t xStreamBuffer);

/**
 * @brief See FreeRTOS stream_buffer.h
 *
 * @param xStreamBuffer
 * @return
 */
size_t streamBufferSpacesAvailable(StreamBufferHandle_t xStreamBuffer);

/**
 * @brief See FreeRTOS stream_buffer.h
 *
 * @param xStreamBuffer
 * @return
 */
BaseType_t streamBufferReset(StreamBufferHandle_t xStreamBuffer);

/**
 * @brief See FreeRTOS stream_buffer.h
 *
 * @param xStreamBuffer
 * @return
 */
BaseType_t streamBufferIsEmpty(StreamBufferHandle_t xStreamBuffer);

/**
 * @brief See FreeRTOS stream_buffer.h
 *
 * @param xStreamBuffer
 * @return
 */
BaseType_t streamBufferIsFull(StreamBufferHandle_t xStreamBuffer);

/* message_buffer.h abstraction */

/**
 * @brief See FreeRTOS message_buffer.h
 *
 * @param xBufferSizeBytes
 * @param pucMessageBufferStorageArea
 * @param pxStaticMessageBuffer
 * @return
 */
MessageBufferHandle_t messageBufferCreateStatic(size_t xBufferSizeBytes, uint8_t* pucMessageBufferStorageArea,
                                                StaticMessageBuffer_t* pxStaticMessageBuffer);

/**
 * @brief See FreeRTOS message_buffer.h
 *
 * @param xMessageBuffer
 * @param pvTxData
 * @param xDataLengthBytes
 * @param xTicksToWait
 * @return
 */
size_t messageBufferSend(MessageBufferHandle_t xMessageBuffer, const void* pvTxData, size_t xDataLengthBytes, TickType_t xTicksToWait);

/**
 * @brief See FreeRTOS message_buffer.h
 *
 * @param xMessageBuffer
 * @param pvTxData
 * @param xDataLengthBytes
 * @param pxHigherPriorityTaskWoken
 * @return
 */
size_t messageBufferSendFromISR(MessageBufferHandle_t xMessageBuffer, const void* pvTxData, size_t xDataLengthBytes,
                                BaseType_t* pxHigherPriorityTaskWoken);

/**
 * @brief See FreeRTOS message_buffer.h
 *
 * @param xMessageBuffer
 * @param pvRxData
 * @param xBufferLengthBytes
 * @param xTicksToWait
 * @return
 */
size_t messageBufferReceive(MessageBufferHandle_t xMessageBuffer, void* pvRxData, size_t xBufferLengthBytes, TickType_t xTicksToWait);

/**
 * @brief See FreeRTOS message_buffer.h
 *
 * @param xMessageBuffer
 * @param pvRxData
 * @param xBufferLengthBytes
 * @param pxHigherPriorityTaskWoken
 * @return
 */
size_t messageBufferReceiveFromISR(MessageBufferHandle_t xMessageBuffer, void* pvRxData, size_t xBufferLengthBytes,
                                   BaseType_t* pxHigherPriorityTaskWoken);

/**
 * @brief See FreeRTOS message_buffer.h
 *
 * @param xMessageBuffer
 */
void messageBufferDelete(MessageBufferHandle_t xMessageBuffer);

/**
 * @brief See FreeRTOS message_buffer.h
 *
 * @param xMessageBuffer
 * @return
 */
size_t messageBufferSpacesAvailable(MessageBufferHandle_t xMessageBuffer);

/**
 * @brief See FreeRTOS message_buffer.h
 *
 * @param xMessageBuffer
 * @return
 */
BaseType_t messageBufferReset(MessageBufferHandle_t xMessageBuffer);

/**
 * @brief See FreeRTOS message_buffer.h
 *
 * @param xMessageBuffer
 * @return
 */
BaseType_t messageBufferIsEmpty(MessageBufferHandle_t xMessageBuffer);

/**
 * @brief See FreeRTOS message_buffer.h
 *
 * @param xMessageBuffer
 * @return
 */
BaseType_t messageBufferIsFull(MessageBufferHandle_t xMessageBuffer);

/* semphr.h abstraction */

/**
 * @brief See FreeRTOS semphr.h
 *
 * @param uxMaxCount
 * @param uxInitialCount
 * @param pxSemaphoreBuffer
 * @return
 */
SemaphoreHandle_t semaphoreCreateCountingStatic(UBaseType_t uxMaxCount, UBaseType_t uxInitialCount, StaticSemaphore_t* pxSemaphoreBuffer);

/**
 * @brief See FreeRTOS semphr.h
 *
 * @param pxSemaphoreBuffer
 * @return
 */
SemaphoreHandle_t semaphoreCreateBinaryStatic(StaticSemaphore_t* pxSemaphoreBuffer);

/**
 * @brief See FreeRTOS semphr.h
 *
 * @param pxMutexBuffer
 * @return
 */
SemaphoreHandle_t mutexCreateStatic(StaticSemaphore_t* pxMutexBuffer);

/**
 * @brief See FreeRTOS semphr.h
 *
 * @param xSemaphore
 */
void semaphoreDelete(SemaphoreHandle_t xSemaphore);

/**
 * @brief See FreeRTOS semphr.h
 *
 * @param xSemaphore
 * @param xBlockTime
 * @return
 */
BaseType_t semaphoreTake(SemaphoreHandle_t xSemaphore, TickType_t xBlockTime);

/**
 * @brief See FreeRTOS semphr.h
 *
 * @param xSemaphore
 * @param pxHigherPriorityTaskWoken
 * @return
 */
BaseType_t semaphoreTakeFromISR(SemaphoreHandle_t xSemaphore, BaseType_t* pxHigherPriorityTaskWoken);

/**
 * @brief See FreeRTOS semphr.h
 *
 * @param xSemaphore
 * @return
 */
BaseType_t semaphoreGive(SemaphoreHandle_t xSemaphore);

/**
 * @brief See FreeRTOS semphr.h
 *
 * @param xSemaphore
 * @param pxHigherPriorityTaskWoken
 * @return
 */
BaseType_t semaphoreGiveFromISR(SemaphoreHandle_t xSemaphore, BaseType_t* pxHigherPriorityTaskWoken);

/**
 * @brief See FreeRTOS semphr.h
 *
 * @param xSemaphore
 * @return
 */
UBaseType_t semaphoreGetCount(SemaphoreHandle_t xSemaphore);

/**
 * @brief Implements getCount for semaphores to be used inside ISR
 *
 * This uses the original FreeRTOS API, however this interface does not exist
 * in the original FreeRTOS semphr.h API
 *
 * @param xSemaphore
 * @return
 */
UBaseType_t semaphoreGetCountFromISR(SemaphoreHandle_t xSemaphore);

/* portmacro.h abstraction */

/**
 * @brief Request a context switch in ISR
 *
 * For further details see FreeRTOS portmacro.h portYIELD_FROM_ISR
 *
 * @param xHigherPriorityTaskWoken
 */
void yieldFromISR(BaseType_t xHigherPriorityTaskWoken);

#ifdef __cplusplus
}  // extern "C"
}  // namespace osabstraction
#endif  // __cplusplus

#endif /* OSABSTRACTION_OSABSTRACTION_HPP */
