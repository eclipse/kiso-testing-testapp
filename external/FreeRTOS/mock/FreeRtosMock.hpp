/**
 * @file FreeRtosMock.hpp
 * @author Kris Wolfgramm
 *
 * @brief Mocks FreeRTOS functionality
 *
 * @copyright Copyright 2019 Robert Bosch GmbH. All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 */
#ifndef MOCK_FREERTOSMOCK_HPP
#define MOCK_FREERTOSMOCK_HPP

#include <gmock/gmock.h>

#include <FreeRTOS.h>
#include <event_groups.h>
#include <message_buffer.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>

#include <memory>

namespace freertos {

class Mock {
public:
  Mock();
  virtual ~Mock();

  /* task.h abstraction */

  MOCK_METHOD(char*, pcTaskGetName, (TaskHandle_t));
  MOCK_METHOD(TickType_t, xTaskGetTickCount, ());
  MOCK_METHOD(TickType_t, xTaskGetTickCountFromISR, ());
  MOCK_METHOD(BaseType_t, xTaskGetSchedulerState, ());
  MOCK_METHOD(void, vTaskSuspend, (TaskHandle_t));
  MOCK_METHOD(void, vTaskSuspendAll, ());
  MOCK_METHOD(void, vTaskResume, (TaskHandle_t));
  MOCK_METHOD(void, vTaskDelay, (const TickType_t));
  MOCK_METHOD(void, vTaskDelayUntil, (TickType_t*, TickType_t));
  MOCK_METHOD(TaskHandle_t, xTaskCreateStatic,
              (TaskFunction_t, const char*, uint32_t, void*, UBaseType_t,
               StackType_t*, StaticTask_t*));
  MOCK_METHOD(void, vTaskDelete, (TaskHandle_t));
  MOCK_METHOD(BaseType_t, xTaskGenericNotify,
              (TaskHandle_t, uint32_t, eNotifyAction, uint32_t*));
  MOCK_METHOD(BaseType_t, xTaskGenericNotifyFromISR,
              (TaskHandle_t, uint32_t, eNotifyAction, uint32_t*, BaseType_t*));
  MOCK_METHOD(BaseType_t, xTaskNotifyWait,
              (uint32_t, uint32_t, uint32_t*, TickType_t));
  MOCK_METHOD(uint32_t, ulTaskNotifyTake, (BaseType_t, TickType_t));
  MOCK_METHOD(void, vTaskNotifyGiveFromISR, (TaskHandle_t, BaseType_t*));
  MOCK_METHOD(TaskHandle_t, xTaskGetCurrentTaskHandle, ());
  MOCK_METHOD(UBaseType_t, uxTaskPriorityGet, (const TaskHandle_t));
  MOCK_METHOD(void, vTaskPrioritySet, (TaskHandle_t, UBaseType_t));
  MOCK_METHOD(eTaskState, eTaskGetState, (TaskHandle_t));
  MOCK_METHOD(UBaseType_t, uxTaskGetNumberOfTasks, ());

  /* queue.h abstraction QueueSets */

  MOCK_METHOD(QueueSetMemberHandle_t, xQueueSelectFromSet,
              (QueueSetHandle_t, TickType_t));
  MOCK_METHOD(BaseType_t, xQueueAddToSet,
              (QueueSetMemberHandle_t, QueueSetHandle_t));
  MOCK_METHOD(BaseType_t, xQueueRemoveFromSet,
              (QueueSetMemberHandle_t, QueueSetHandle_t));

  /* queue.h abstraction */

  MOCK_METHOD(QueueHandle_t, xQueueGenericCreateStatic,
              (UBaseType_t, UBaseType_t, uint8_t*, StaticQueue_t*, uint8_t));
  MOCK_METHOD(QueueHandle_t, xQueueCreateMutexStatic,
              (uint8_t, StaticQueue_t*));
  MOCK_METHOD(BaseType_t, xQueueGenericSend,
              (QueueHandle_t, const void*, TickType_t, BaseType_t));
  MOCK_METHOD(BaseType_t, xQueueGenericSendFromISR,
              (QueueHandle_t, const void*, BaseType_t*, BaseType_t));
  MOCK_METHOD(BaseType_t, xQueueReceive, (QueueHandle_t, void*, TickType_t));
  MOCK_METHOD(BaseType_t, xQueueReceiveFromISR,
              (QueueHandle_t, void*, BaseType_t*));
  MOCK_METHOD(BaseType_t, xQueueGenericReset, (QueueHandle_t, BaseType_t));
  MOCK_METHOD(void, vQueueDelete, (QueueHandle_t));
  MOCK_METHOD(TaskHandle_t, xQueueGetMutexHolder, (QueueHandle_t));
  MOCK_METHOD(BaseType_t, xQueueTakeMutexRecursive, (QueueHandle_t, TickType_t));
  MOCK_METHOD(BaseType_t, xQueueGiveMutexRecursive, (QueueHandle_t));
  MOCK_METHOD(UBaseType_t, uxQueueSpacesAvailable, (QueueHandle_t));
  MOCK_METHOD(UBaseType_t, uxQueueMessagesWaiting, (QueueHandle_t));
  MOCK_METHOD(UBaseType_t, uxQueueMessagesWaitingFromISR, (QueueHandle_t));

#if (configQUEUE_REGISTRY_SIZE > 0)
  MOCK_METHOD(void, vQueueAddToRegistry, (QueueHandle_t, const char*));
  MOCK_METHOD(void, vQueueUnregisterQueue, (QueueHandle_t));
  MOCK_METHOD(const char*, pcQueueGetName, (QueueHandle_t));
#endif

  /* stream_buffer.h abstraction */

  MOCK_METHOD(StreamBufferHandle_t, xStreamBufferGenericCreateStatic,
              (size_t, size_t, BaseType_t, uint8_t*, StaticStreamBuffer_t*));
  MOCK_METHOD(size_t, xStreamBufferSend,
              (StreamBufferHandle_t, const void*, size_t, TickType_t));
  MOCK_METHOD(size_t, xStreamBufferSendFromISR,
              (StreamBufferHandle_t, const void*, size_t, BaseType_t*));
  MOCK_METHOD(size_t, xStreamBufferReceive,
              (StreamBufferHandle_t, void*, size_t, TickType_t));
  MOCK_METHOD(size_t, xStreamBufferReceiveFromISR,
              (StreamBufferHandle_t, void*, size_t, BaseType_t*));
  MOCK_METHOD(void, vStreamBufferDelete, (StreamBufferHandle_t));
  MOCK_METHOD(size_t, xStreamBufferBytesAvailable, (StreamBufferHandle_t));
  MOCK_METHOD(size_t, xStreamBufferSpacesAvailable, (StreamBufferHandle_t));
  MOCK_METHOD(BaseType_t, xStreamBufferReset, (StreamBufferHandle_t));
  MOCK_METHOD(BaseType_t, xStreamBufferIsEmpty, (StreamBufferHandle_t));
  MOCK_METHOD(BaseType_t, xStreamBufferIsFull, (StreamBufferHandle_t));

  /* semphr.h abstraction */

  MOCK_METHOD(SemaphoreHandle_t, xQueueCreateCountingSemaphoreStatic,
              (UBaseType_t, UBaseType_t, StaticSemaphore_t*));
  MOCK_METHOD(BaseType_t, xQueueSemaphoreTake, (SemaphoreHandle_t, TickType_t));
  MOCK_METHOD(BaseType_t, xQueueGiveFromISR, (SemaphoreHandle_t, BaseType_t*));

  /* event_groups.h abstraction */

  MOCK_METHOD(EventBits_t, xEventGroupSetBits,
              (EventGroupHandle_t, const EventBits_t));
  MOCK_METHOD(EventBits_t, xEventGroupWaitBits,
              (EventGroupHandle_t, const EventBits_t, const BaseType_t,
               const BaseType_t, TickType_t));
  MOCK_METHOD(EventBits_t, xEventGroupClearBits,
              (EventGroupHandle_t, EventBits_t));
  MOCK_METHOD(EventGroupHandle_t, xEventGroupCreateStatic,
              (StaticEventGroup_t*));
  MOCK_METHOD(void, vEventGroupSetBitsCallback,
              (void*, const uint32_t));

  /* portmacro.h abstraction */
  MOCK_METHOD(void, vPortEnterCritical, ());
  MOCK_METHOD(void, vPortExitCritical, ());
  MOCK_METHOD(void, vClearInterruptMask, (uint32_t));
  MOCK_METHOD(uint32_t, ulSetInterruptMask, ());
  MOCK_METHOD(BaseType_t, xPortIsInsideInterrupt, ());

  /* timers.h abstraction */
  MOCK_METHOD(TimerHandle_t, xTimerCreateStatic,
              (const char* const, const TickType_t, const UBaseType_t,
               void* const, TimerCallbackFunction_t, StaticTimer_t*));
  MOCK_METHOD(BaseType_t, xTimerGenericCommand,
              (TimerHandle_t, const BaseType_t, const TickType_t,
               BaseType_t* const, const TickType_t));
  MOCK_METHOD(void*, pvTimerGetTimerID, (const TimerHandle_t));
  MOCK_METHOD(TickType_t, xTimerGetPeriod, (TimerHandle_t));
  MOCK_METHOD(BaseType_t, xTimerIsTimerActive, (TimerHandle_t));
  MOCK_METHOD(BaseType_t, xTimerPendFunctionCallFromISR,
              (PendedFunction_t, void*, uint32_t, BaseType_t*));
};

extern std::unique_ptr<Mock> mock;

}  // namespace freertos

#endif /* MOCK_FREERTOSMOCK_HPP */
