/**
 * @file FreeRtosMock.cpp
 * @author Kris Wolfgramm
 *
 * @brief See *.hpp for more details
 *
 * @copyright Copyright 2019 Robert Bosch GmbH. All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 */
#include <FreeRtosMock.hpp>

namespace freertos {

std::unique_ptr<Mock> mock{};

// See https://github.com/google/googletest/blob/master/googlemock/docs/cook_book.md#making-the-compilation-faster
Mock::Mock() {}
Mock::~Mock() {}

/* task.h abstraction */

extern "C" TaskHandle_t xTaskGetCurrentTaskHandle() {
  assert(mock);
  return mock->xTaskGetCurrentTaskHandle();
}

extern "C" char* pcTaskGetName(TaskHandle_t xTaskToQuery) {
  assert(mock);
  return mock->pcTaskGetName(xTaskToQuery);
}

extern "C" TickType_t xTaskGetTickCount() {
  assert(mock);
  return mock->xTaskGetTickCount();
}

extern "C" TickType_t xTaskGetTickCountFromISR() {
  assert(mock);
  return mock->xTaskGetTickCountFromISR();
}

extern "C" BaseType_t xTaskGetSchedulerState() {
  assert(mock);
  return mock->xTaskGetSchedulerState();
}

extern "C" void vTaskSuspend(TaskHandle_t xTaskToSuspend) {
  assert(mock);
  mock->vTaskSuspend(xTaskToSuspend);
}

extern "C" void vTaskSuspendAll() {
  assert(mock);
  mock->vTaskSuspendAll();
}

extern "C" void vTaskResume(TaskHandle_t xTaskToResume) {
  assert(mock);
  mock->vTaskResume(xTaskToResume);
}

extern "C" void vTaskDelay(const TickType_t xTicksToDelay) {
  assert(mock);
  mock->vTaskDelay(xTicksToDelay);
}

extern "C" void vTaskDelayUntil(TickType_t* const pxPreviousWakeTime,
                                const TickType_t xTimeIncrement) {
  assert(mock);
  mock->vTaskDelayUntil(pxPreviousWakeTime, xTimeIncrement);
}

extern "C" TaskHandle_t
xTaskCreateStatic(TaskFunction_t pxTaskCode, const char* const pcName,
                  const uint32_t ulStackDepth, void* const pvParameters,
                  UBaseType_t uxPriority, StackType_t* const puxStackBuffer,
                  StaticTask_t* const pxTaskBuffer) {
  assert(mock);
  return mock->xTaskCreateStatic(pxTaskCode, pcName, ulStackDepth, pvParameters,
                                 uxPriority, puxStackBuffer, pxTaskBuffer);
}

extern "C" void vTaskDelete(TaskHandle_t xTaskToDelete) {
  assert(mock);
  mock->vTaskDelete(xTaskToDelete);
}

extern "C" BaseType_t
xTaskGenericNotify(TaskHandle_t xTaskToNotify, uint32_t ulValue,
                   eNotifyAction eAction,
                   uint32_t* pulPreviousNotificationValue) {
  assert(mock);
  return mock->xTaskGenericNotify(xTaskToNotify, ulValue, eAction,
                                  pulPreviousNotificationValue);
}

extern "C" BaseType_t
xTaskGenericNotifyFromISR(TaskHandle_t xTaskToNotify, uint32_t ulValue,
                          eNotifyAction eAction,
                          uint32_t* pulPreviousNotificationValue,
                          BaseType_t* pxHigherPriorityTaskWoken) {
  assert(mock);
  return mock->xTaskGenericNotifyFromISR(xTaskToNotify, ulValue, eAction,
                                  pulPreviousNotificationValue, pxHigherPriorityTaskWoken);
}

extern "C" BaseType_t
xTaskNotifyWait(uint32_t ulBitsToClearOnEntry,
                uint32_t ulBitsToClearOnExit,
                uint32_t* pulNotificationValue,
                TickType_t xTicksToWait) {
  assert(mock);
  return mock->xTaskNotifyWait(ulBitsToClearOnEntry, ulBitsToClearOnExit,
                                  pulNotificationValue, xTicksToWait);
}

extern "C" uint32_t ulTaskNotifyTake(BaseType_t xClearCountOnExit,
                                     TickType_t xTicksToWait) {
  assert(mock);
  return mock->ulTaskNotifyTake(xClearCountOnExit, xTicksToWait);
}

extern "C" void vTaskNotifyGiveFromISR(TaskHandle_t xTaskToNotify,
                                       BaseType_t *pxHigherPriorityTaskWoken) {
  assert(mock);
  mock->vTaskNotifyGiveFromISR(xTaskToNotify, pxHigherPriorityTaskWoken);
}

extern "C" UBaseType_t uxTaskPriorityGet(const TaskHandle_t pxTask) {
  assert(mock);
  return mock->uxTaskPriorityGet(pxTask);
}

extern "C" void vTaskPrioritySet(TaskHandle_t pxTask, 
                                 UBaseType_t uxNewPriority) {
  assert(mock);
  mock->vTaskPrioritySet(pxTask, uxNewPriority);
}

/* queue.h abstraction QueueSets */

extern "C" QueueSetMemberHandle_t
xQueueSelectFromSet(QueueSetHandle_t xQueueSet, TickType_t xBlockTimeTicks) {
  assert(mock);
  return mock->xQueueSelectFromSet(xQueueSet, xBlockTimeTicks);
}

extern "C" BaseType_t xQueueAddToSet(QueueSetMemberHandle_t xQueueOrSemaphore,
                                     QueueSetHandle_t xQueueSet) {
  assert(mock);
  return mock->xQueueAddToSet(xQueueOrSemaphore, xQueueSet);
}

extern "C" BaseType_t
xQueueRemoveFromSet(QueueSetMemberHandle_t xQueueOrSemaphore,
                    QueueSetHandle_t xQueueSet) {
  assert(mock);
  return mock->xQueueRemoveFromSet(xQueueOrSemaphore, xQueueSet);
}

extern "C" eTaskState eTaskGetState(TaskHandle_t pxTask) {
  assert(mock);
  return mock->eTaskGetState(pxTask);
}

extern "C" UBaseType_t uxTaskGetNumberOfTasks() {
  assert(mock);
  return mock->uxTaskGetNumberOfTasks();
}

/* queue.h abstraction */

extern "C" QueueHandle_t xQueueGenericCreateStatic(
    const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize,
    uint8_t* pucQueueStorage, StaticQueue_t* pxStaticQueue,
    const uint8_t ucQueueType) {
  assert(mock);
  return mock->xQueueGenericCreateStatic(
      uxQueueLength, uxItemSize, pucQueueStorage, pxStaticQueue, ucQueueType);
}

extern "C" QueueHandle_t xQueueCreateMutexStatic(const uint8_t ucQueueType,
                                                 StaticQueue_t* pxStaticQueue) {
  assert(mock);
  return mock->xQueueCreateMutexStatic(ucQueueType, pxStaticQueue);
}

extern "C" BaseType_t xQueueGenericSend(QueueHandle_t xQueue,
                                        const void* const pvItemToQueue,
                                        TickType_t xTicksToWait,
                                        const BaseType_t xCopyPosition) {
  assert(mock);
  return mock->xQueueGenericSend(xQueue, pvItemToQueue, xTicksToWait,
                                 xCopyPosition);
}

extern "C" BaseType_t
xQueueGenericSendFromISR(QueueHandle_t xQueue, const void* const pvItemToQueue,
                         BaseType_t* const pxHigherPriorityTaskWoken,
                         const BaseType_t xCopyPosition) {
  assert(mock);
  return mock->xQueueGenericSendFromISR(
      xQueue, pvItemToQueue, pxHigherPriorityTaskWoken, xCopyPosition);
}

extern "C" BaseType_t xQueueReceive(QueueHandle_t xQueue, void* pvBuffer,
                                    TickType_t xTicksToWait) {
  assert(mock);
  return mock->xQueueReceive(xQueue, pvBuffer, xTicksToWait);
}

extern "C" BaseType_t
xQueueReceiveFromISR(QueueHandle_t xQueue, void* pvBuffer,
                     BaseType_t* pxHigherPriorityTaskWoken) {
  assert(mock);
  return mock->xQueueReceiveFromISR(xQueue, pvBuffer,
                                    pxHigherPriorityTaskWoken);
}

extern "C" BaseType_t xQueueGenericReset(QueueHandle_t xQueue,
                                         BaseType_t xNewQueue) {
  assert(mock);
  return mock->xQueueGenericReset(xQueue, xNewQueue);
}

extern "C" void vQueueDelete(QueueHandle_t xQueue) {
  assert(mock);
  mock->vQueueDelete(xQueue);
}

#if (configQUEUE_REGISTRY_SIZE > 0)
extern "C" void vQueueAddToRegistry(QueueHandle_t xQueue, const char* name) {
  assert(mock);
  mock->vQueueAddToRegistry(xQueue, name);
}

extern "C" void vQueueUnregisterQueue(QueueHandle_t xQueue) {
  assert(mock);
  mock->vQueueUnregisterQueue(xQueue);
}

extern "C" const char* pcQueueGetName(QueueHandle_t xQueue) {
  assert(mock);
  return mock->pcQueueGetName(xQueue);
}
#endif

extern "C" TaskHandle_t xQueueGetMutexHolder(QueueHandle_t xSemaphore) {
  assert(mock);
  return mock->xQueueGetMutexHolder(xSemaphore);
}

extern "C" BaseType_t xQueueTakeMutexRecursive(QueueHandle_t xMutex, TickType_t xTicksToWait) {
  assert(mock);
  return mock->xQueueTakeMutexRecursive(xMutex, xTicksToWait);
}

extern "C" BaseType_t xQueueGiveMutexRecursive(QueueHandle_t xMutex) {
  assert(mock);
  return mock->xQueueGiveMutexRecursive(xMutex);
}

extern "C" UBaseType_t uxQueueSpacesAvailable(QueueHandle_t xQueue) {
  assert(mock);
  return mock->uxQueueSpacesAvailable(xQueue);
}

extern "C" UBaseType_t uxQueueMessagesWaiting(const QueueHandle_t xQueue) {
  assert(mock);
  return mock->uxQueueMessagesWaiting(xQueue);
}

extern "C" UBaseType_t
uxQueueMessagesWaitingFromISR(const QueueHandle_t xQueue) {
  assert(mock);
  return mock->uxQueueMessagesWaitingFromISR(xQueue);
}

/* stream_buffer.h abstraction */

extern "C" StreamBufferHandle_t xStreamBufferGenericCreateStatic(
    size_t xBufferSizeBytes, size_t xTriggerLevelBytes,
    BaseType_t xIsMessageBuffer, uint8_t* const pucStreamBufferStorageArea,
    StaticStreamBuffer_t* const pxStaticStreamBuffer) {
  assert(mock);
  return mock->xStreamBufferGenericCreateStatic(
      xBufferSizeBytes, xTriggerLevelBytes, xIsMessageBuffer,
      pucStreamBufferStorageArea, pxStaticStreamBuffer);
}

extern "C" size_t xStreamBufferSend(StreamBufferHandle_t xStreamBuffer,
                                    const void* pvTxData,
                                    size_t xDataLengthBytes,
                                    TickType_t xTicksToWait) {
  assert(mock);
  return mock->xStreamBufferSend(xStreamBuffer, pvTxData, xDataLengthBytes,
                                 xTicksToWait);
}

extern "C" size_t
xStreamBufferSendFromISR(StreamBufferHandle_t xStreamBuffer,
                         const void* pvTxData, size_t xDataLengthBytes,
                         BaseType_t* const pxHigherPriorityTaskWoken) {
  assert(mock);
  return mock->xStreamBufferSendFromISR(
      xStreamBuffer, pvTxData, xDataLengthBytes, pxHigherPriorityTaskWoken);
}

extern "C" size_t xStreamBufferReceive(StreamBufferHandle_t xStreamBuffer,
                                       void* pvRxData,
                                       size_t xBufferLengthBytes,
                                       TickType_t xTicksToWait) {
  assert(mock);
  return mock->xStreamBufferReceive(xStreamBuffer, pvRxData, xBufferLengthBytes,
                                    xTicksToWait);
}

extern "C" size_t
xStreamBufferReceiveFromISR(StreamBufferHandle_t xStreamBuffer, void* pvRxData,
                            size_t xBufferLengthBytes,
                            BaseType_t* const pxHigherPriorityTaskWoken) {
  assert(mock);
  return mock->xStreamBufferReceiveFromISR(
      xStreamBuffer, pvRxData, xBufferLengthBytes, pxHigherPriorityTaskWoken);
}

extern "C" void vStreamBufferDelete(StreamBufferHandle_t xStreamBuffer) {
  assert(mock);
  mock->vStreamBufferDelete(xStreamBuffer);
}

extern "C" size_t
xStreamBufferBytesAvailable(StreamBufferHandle_t xStreamBuffer) {
  assert(mock);
  return mock->xStreamBufferBytesAvailable(xStreamBuffer);
}

extern "C" size_t
xStreamBufferSpacesAvailable(StreamBufferHandle_t xStreamBuffer) {
  assert(mock);
  return mock->xStreamBufferSpacesAvailable(xStreamBuffer);
}

extern "C" BaseType_t xStreamBufferReset(StreamBufferHandle_t xStreamBuffer) {
  assert(mock);
  return mock->xStreamBufferReset(xStreamBuffer);
}

extern "C" BaseType_t xStreamBufferIsEmpty(StreamBufferHandle_t xStreamBuffer) {
  assert(mock);
  return mock->xStreamBufferIsEmpty(xStreamBuffer);
}

extern "C" BaseType_t xStreamBufferIsFull(StreamBufferHandle_t xStreamBuffer) {
  assert(mock);
  return mock->xStreamBufferIsFull(xStreamBuffer);
}

/* semphr.h abstraction */

extern "C" SemaphoreHandle_t
xQueueCreateCountingSemaphoreStatic(UBaseType_t uxMaxCount,
                                    UBaseType_t uxInitialCount,
                                    StaticSemaphore_t* pxSemaphoreBuffer) {
  assert(mock);
  return mock->xQueueCreateCountingSemaphoreStatic(uxMaxCount, uxInitialCount,
                                                   pxSemaphoreBuffer);
}

extern "C" BaseType_t xQueueSemaphoreTake(SemaphoreHandle_t xSemaphore,
                                          TickType_t xBlockTime) {
  assert(mock);
  return mock->xQueueSemaphoreTake(xSemaphore, xBlockTime);
}

extern "C" BaseType_t xQueueGiveFromISR(SemaphoreHandle_t xSemaphore,
                                        BaseType_t* pxHigherPriorityTaskWoken) {
  assert(mock);
  return mock->xQueueGiveFromISR(xSemaphore, pxHigherPriorityTaskWoken);
}

/* event_groups.h abstraction */

extern "C" EventBits_t xEventGroupSetBits(EventGroupHandle_t xEventGroup,
                                          const EventBits_t uxBitsToSet) {
  assert(mock);
  return mock->xEventGroupSetBits(xEventGroup, uxBitsToSet);
}

extern "C" EventBits_t xEventGroupWaitBits(EventGroupHandle_t xEventGroup,
                                           const EventBits_t uxBitsToWaitFor,
                                           const BaseType_t xClearOnExit,
                                           const BaseType_t xWaitForAllBits,
                                           TickType_t xTicksToWait) {
  assert(mock);
  return mock->xEventGroupWaitBits(xEventGroup, uxBitsToWaitFor, xClearOnExit,
                                   xWaitForAllBits, xTicksToWait);
}

extern "C" EventBits_t xEventGroupClearBits(EventGroupHandle_t xEventGroup,
                                            const EventBits_t uxBitsToClear) {
  assert(mock);
  return mock->xEventGroupClearBits(xEventGroup, uxBitsToClear);
}

extern "C" EventGroupHandle_t
xEventGroupCreateStatic(StaticEventGroup_t* pxEventGroupBuffer) {
  assert(mock);
  return mock->xEventGroupCreateStatic(pxEventGroupBuffer);
}

extern "C" void
vEventGroupSetBitsCallback(void* pvEventGroup, const uint32_t ulBitsToSet) {
  assert(mock);
  return mock->vEventGroupSetBitsCallback(pvEventGroup, ulBitsToSet);
}

/* portmacro.h abstraction */

extern "C" void vPortEnterCritical() {
  assert(mock);
  return mock->vPortEnterCritical();
}

extern "C" void vPortExitCritical() {
  assert(mock);
  return mock->vPortExitCritical();
}

extern "C" void vClearInterruptMask(uint32_t ulMask) {
  assert(mock);
  mock->vClearInterruptMask(ulMask);
}

extern "C" uint32_t ulSetInterruptMask() {
  assert(mock);
  return mock->ulSetInterruptMask();
}

extern "C" BaseType_t xPortIsInsideInterrupt() {
  assert(mock);
  return mock->xPortIsInsideInterrupt();
}

/* timers.h abstraction */

extern "C" TimerHandle_t xTimerCreateStatic(const char* const pcTimerName,
                                            const TickType_t xTimerPeriodInTicks,
                                            const UBaseType_t uxAutoReload,
                                            void* const pvTimerID,
                                            TimerCallbackFunction_t pxCallbackFunction,
                                            StaticTimer_t* pxTimerBuffer) {
  assert(mock);
  return mock->xTimerCreateStatic(pcTimerName, xTimerPeriodInTicks, uxAutoReload,
                                  pvTimerID, pxCallbackFunction, pxTimerBuffer);
}

extern "C" BaseType_t xTimerGenericCommand(TimerHandle_t xTimer,
                                           const BaseType_t xCommandID,
                                           const TickType_t xOptionalValue,
                                           BaseType_t* const pxHigherPriorityTaskWoken,
                                           const TickType_t xTicksToWait) {
  assert(mock);
  return mock->xTimerGenericCommand(xTimer, xCommandID, xOptionalValue,
                                    pxHigherPriorityTaskWoken, xTicksToWait);
}

extern "C" void* pvTimerGetTimerID(const TimerHandle_t xTimer) {
  assert(mock);
  return mock->pvTimerGetTimerID(xTimer);
}

extern "C" TickType_t xTimerGetPeriod(TimerHandle_t xTimer) {
  assert(mock);
  return mock->xTimerGetPeriod(xTimer);
}

extern "C" BaseType_t xTimerIsTimerActive(TimerHandle_t xTimer) {
  assert(mock);
  return mock->xTimerIsTimerActive(xTimer);
}

extern "C" BaseType_t xTimerPendFunctionCallFromISR(PendedFunction_t xFunctionToPend,
                                                    void* pvParameter1,
                                                    uint32_t ulParameter2,
                                                    BaseType_t* pxHigherPriorityTaskWoken) {
  assert(mock);
  return mock->xTimerPendFunctionCallFromISR(xFunctionToPend, pvParameter1,
                                             ulParameter2,
                                             pxHigherPriorityTaskWoken);
}

}  // namespace freertos
