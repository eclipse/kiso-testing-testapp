/**
 * @file include/OsAbstraction/OsAbstraction/OsAbstractionTypes.hpp
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 */

#ifndef OSABSTRACTION_OSABSTRACTIONTYPES_HPP
#define OSABSTRACTION_OSABSTRACTIONTYPES_HPP

#include <cstdint>
#include <cstddef>

// FreeRTOSConfig.h
/// see FreeRTOS API documentation
#define configMAX_TASK_NAME_LEN (10)  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
#define configTICK_RATE_HZ ((portTickType)1000)  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS

// projdefs.h
/// see FreeRTOS API documentation
typedef void (*TaskFunction_t)(void*);  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
#define pdMS_TO_TICKS(xTimeInMs) ((TickType_t)(((TickType_t)(xTimeInMs) * (TickType_t)configTICK_RATE_HZ) / (TickType_t)1000))  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
#define pdFALSE ((BaseType_t)0)  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
#define pdTRUE ((BaseType_t)1)  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS

// portmacro.h
/// see FreeRTOS API documentation
typedef uint32_t TickType_t;  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
typedef long BaseType_t;  //NOLINT(google-runtime-int, modernize-use-using,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
typedef unsigned long UBaseType_t;  //NOLINT(google-runtime-int, modernize-use-using,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
#define portSTACK_TYPE size_t  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
typedef portSTACK_TYPE StackType_t;  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
#define portMAX_DELAY (TickType_t)0xffffffffUL  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
#define portTICK_PERIOD_MS ((TickType_t)1000 / configTICK_RATE_HZ)  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS

// FreeRTOS.h
/// see FreeRTOS API documentation
struct xSTATIC_MINI_LIST_ITEM {  //NOLINT(readability-identifier-naming,readability-identifier-naming) defined by FreeRTOS
#if (configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES == 1)
  TickType_t xDummy1;
#endif
  /// see FreeRTOS API documentation
  TickType_t xDummy2;
  /// see FreeRTOS API documentation
  void* pvDummy3[2];  //NOLINT(modernize-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays) defined by FreeRTOS
};
typedef struct xSTATIC_MINI_LIST_ITEM StaticMiniListItem_t;  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS

/// see FreeRTOS API documentation
typedef struct xSTATIC_LIST {  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
#if (configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES == 1)
  TickType_t xDummy1;
#endif
  /// see FreeRTOS API documentation
  UBaseType_t uxDummy2;
  /// see FreeRTOS API documentation
  void* pvDummy3;
  /// see FreeRTOS API documentation
  StaticMiniListItem_t xDummy4;
#if (configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES == 1)
  TickType_t xDummy5;
#endif
} StaticList_t;  //NOLINT(readability-identifier-naming) defined by FreeRTOS

/// see FreeRTOS API documentation
typedef struct xSTATIC_QUEUE {  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
  /// see FreeRTOS API documentation
  void* pvDummy1[3];  //NOLINT(modernize-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays) defined by FreeRTOS

  /// see FreeRTOS API documentation
  union {
    /// see FreeRTOS API documentation
    void* pvDummy2;
    /// see FreeRTOS API documentation
    UBaseType_t uxDummy2;
  } u;

  /// see FreeRTOS API documentation
  StaticList_t xDummy3[2];  //NOLINT(modernize-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays) defined by FreeRTOS
  /// see FreeRTOS API documentation
  UBaseType_t uxDummy4[3];  //NOLINT(modernize-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays) defined by FreeRTOS
  /// see FreeRTOS API documentation
  uint8_t ucDummy5[2];  //NOLINT(modernize-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays) defined by FreeRTOS

#if ((configSUPPORT_STATIC_ALLOCATION == 1) && (configSUPPORT_DYNAMIC_ALLOCATION == 1))
  uint8_t ucDummy6;
#endif

#if (configUSE_QUEUE_SETS == 1)
  void* pvDummy7;
#endif

#if (configUSE_TRACE_FACILITY == 1)
  UBaseType_t uxDummy8;
  uint8_t ucDummy9;
#endif

} StaticQueue_t;  //NOLINT(readability-identifier-naming,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
typedef StaticQueue_t StaticSemaphore_t;  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS

/// see FreeRTOS API documentation
typedef struct xSTATIC_STREAM_BUFFER {  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
  /// see FreeRTOS API documentation
  size_t uxDummy1[4];  //NOLINT(modernize-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays) defined by FreeRTOS
  /// see FreeRTOS API documentation
  void* pvDummy2[3];  //NOLINT(modernize-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays) defined by FreeRTOS
  /// see FreeRTOS API documentation
  uint8_t ucDummy3;
#if (configUSE_TRACE_FACILITY == 1)
  UBaseType_t uxDummy4;
#endif
} StaticStreamBuffer_t;  //NOLINT(readability-identifier-naming) defined by FreeRTOS

/// see FreeRTOS API documentation
typedef StaticStreamBuffer_t StaticMessageBuffer_t;  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS

/// see FreeRTOS API documentation
#define configSTACK_DEPTH_TYPE uint16_t  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS

/// see FreeRTOS API documentation
struct xSTATIC_LIST_ITEM {  //NOLINT(readability-identifier-naming) defined by FreeRTOS
#if (configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES == 1)
  TickType_t xDummy1;
#endif
  /// see FreeRTOS API documentation
  TickType_t xDummy2;
  /// see FreeRTOS API documentation
  void* pvDummy3[4];  //NOLINT(modernize-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays) defined by FreeRTOS
#if (configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES == 1)
  TickType_t xDummy4;
#endif
};
typedef struct xSTATIC_LIST_ITEM StaticListItem_t;  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS

/// see FreeRTOS API documentation
typedef struct xSTATIC_TCB {  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
  /// see FreeRTOS API documentation
  void* pxDummy1;
#if (portUSING_MPU_WRAPPERS == 1)
  xMPU_SETTINGS xDummy2;
#endif
  /// see FreeRTOS API documentation
  StaticListItem_t xDummy3[2];  //NOLINT(modernize-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays) defined by FreeRTOS
  /// see FreeRTOS API documentation
  UBaseType_t uxDummy5;
  /// see FreeRTOS API documentation
  void* pxDummy6;
  /// see FreeRTOS API documentation
  uint8_t ucDummy7[configMAX_TASK_NAME_LEN];  //NOLINT(modernize-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays) defined by FreeRTOS
#if ((portSTACK_GROWTH > 0) || (configRECORD_STACK_HIGH_ADDRESS == 1))
  void* pxDummy8;
#endif
#if (portCRITICAL_NESTING_IN_TCB == 1)
  UBaseType_t uxDummy9;
#endif
#if (configUSE_TRACE_FACILITY == 1)
  UBaseType_t uxDummy10[2];
#endif
#if (configUSE_MUTEXES == 1)
  UBaseType_t uxDummy12[2];
#endif
#if (configUSE_APPLICATION_TASK_TAG == 1)
  void* pxDummy14;
#endif
#if (configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0)
  void* pvDummy15[configNUM_THREAD_LOCAL_STORAGE_POINTERS];
#endif
#if (configGENERATE_RUN_TIME_STATS == 1)
  uint32_t ulDummy16;
#endif
#if (configUSE_NEWLIB_REENTRANT == 1)
  struct _reent xDummy17;
#endif
#if (configUSE_TASK_NOTIFICATIONS == 1)
  uint32_t ulDummy18;
  uint8_t ucDummy19;
#endif
#if (tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0)  //NOLINT(cppcoreguidelines-macro-usage) defined by FreeRTOS
  uint8_t uxDummy20;
#endif

#if (INCLUDE_xTaskAbortDelay == 1)
  uint8_t ucDummy21;
#endif
#if (configUSE_POSIX_ERRNO == 1)
  int iDummy22;
#endif
} StaticTask_t;  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS

/// see FreeRTOS API documentation
#define portTickType TickType_t  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS

// task.h
struct tskTaskControlBlock; /* The old naming convention is used to prevent breaking kernel aware debuggers. */
/// see FreeRTOS API documentation
typedef struct tskTaskControlBlock* TaskHandle_t;  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
#define taskSCHEDULER_NOT_STARTED ((BaseType_t)1)  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
#define taskSCHEDULER_RUNNING ((BaseType_t)2)  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS

/// see FreeRTOS API documentation
typedef struct xTIME_OUT {  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
                            /// see FreeRTOS API documentation
  BaseType_t xOverflowCount;
  /// see FreeRTOS API documentation
  TickType_t xTimeOnEntering;
} TimeOut_t;  //NOLINT(readability-identifier-naming) defined by FreeRTOS

/// see FreeRTOS API documentation
typedef enum {  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
  //NOLINTNEXTLINE(readability-identifier-naming) defined by FreeRTOS
  eRunning = 0, /* A task is querying the state of itself, so must be running. */
  //NOLINTNEXTLINE(readability-identifier-naming) defined by FreeRTOS
  eReady, /* The task being queried is in a read or pending ready list. */
  //NOLINTNEXTLINE(readability-identifier-naming) defined by FreeRTOS
  eBlocked, /* The task being queried is in the Blocked state. */
  //NOLINTNEXTLINE(readability-identifier-naming) defined by FreeRTOS
  eSuspended, /* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
  //NOLINTNEXTLINE(readability-identifier-naming) defined by FreeRTOS
  eDeleted, /* The task being queried has been deleted, but its TCB has not yet been freed. */
  //NOLINTNEXTLINE(readability-identifier-naming) defined by FreeRTOS
  eInvalid     /* Used as an 'invalid state' value. */
} eTaskState;  //NOLINT(readability-identifier-naming) defined by FreeRTOS

/// see FreeRTOS API documentation
typedef struct xTASK_STATUS {  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
  /// see FreeRTOS API documentation
  TaskHandle_t xHandle; /* The handle of the task to which the rest of the information in the structure relates. */
  /// see FreeRTOS API documentation
  const char* pcTaskName; /* A pointer to the task's name.  This value will be invalid if the task was deleted since the structure was populated! */
                          /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
  /// see FreeRTOS API documentation
  UBaseType_t xTaskNumber; /* A number unique to the task. */
  /// see FreeRTOS API documentation
  eTaskState eCurrentState; /* The state in which the task existed when the structure was populated. */
  /// see FreeRTOS API documentation
  UBaseType_t uxCurrentPriority; /* The priority at which the task was running (may be inherited) when the structure was populated. */
  /// see FreeRTOS API documentation
  UBaseType_t uxBasePriority; /* The priority to which the task will return if the task's current priority has been inherited to avoid unbounded priority inversion when obtaining a mutex.  Only valid if configUSE_MUTEXES is defined as 1 in FreeRTOSConfig.h. */
  /// see FreeRTOS API documentation
  uint32_t ulRunTimeCounter; /* The total run time allocated to the task so far, as defined by the run time stats clock.  See http://www.freertos.org/rtos-run-time-stats.html.  Only valid when configGENERATE_RUN_TIME_STATS is defined as 1 in FreeRTOSConfig.h. */
  /// see FreeRTOS API documentation
  StackType_t* pxStackBase; /* Points to the lowest address of the task's stack area. */
  /// see FreeRTOS API documentation
  configSTACK_DEPTH_TYPE usStackHighWaterMark; /* The minimum amount of stack space that has remained for the task since the task was created.  The closer this value is to zero the closer the task has come to overflowing its stack. */
} TaskStatus_t;                                //NOLINT(readability-identifier-naming,readability-identifier-naming) defined by FreeRTOS

/// see FreeRTOS API documentation
typedef BaseType_t (*TaskHookFunction_t)(void*);  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS

// queue.h
struct QueueDefinition; /* Using old naming convention so as not to break kernel aware debuggers. */
/// see FreeRTOS API documentation
typedef struct QueueDefinition* QueueHandle_t;  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
typedef struct QueueDefinition* QueueSetHandle_t;  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
typedef struct QueueDefinition* QueueSetMemberHandle_t;  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS
/// see FreeRTOS API documentation
#define queueQUEUE_TYPE_SET ((uint8_t)0U)  //NOLINT(cppcoreguidelines-macro-usage,readability-identifier-naming) defined by FreeRTOS

// stream_buffer.h
struct StreamBufferDef_t;
/// see FreeRTOS API documentation
typedef struct StreamBufferDef_t* StreamBufferHandle_t;  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS

// message_buffer.h
/// see FreeRTOS API documentation
typedef void* MessageBufferHandle_t;  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS

// semphr.h
/// see FreeRTOS API documentation
typedef QueueHandle_t SemaphoreHandle_t;  //NOLINT(modernize-use-using,readability-identifier-naming) defined by FreeRTOS

#endif /* OSABSTRACTION_OSABSTRACTIONTYPES_HPP */
