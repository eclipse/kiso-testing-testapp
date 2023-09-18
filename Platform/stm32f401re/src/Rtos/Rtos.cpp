/**
 * @file src/Rtos/Rtos.cpp
 * @brief This Module serves as Integration Layer for freeRTOS and is used to
 * create static task objects within a mpu protected task region managed system.
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

#include <TestAppRtos/Rtos.hpp>
#include <cstring>

#include <FreeRTOS.h>
#include <task.h>
/**
 * Import of the SVC Logger call
 */
namespace logger {
/**
 * \brief Function that sees to having the log message formatted and output.
 * \param[in] arg Pointer to argument vector.
 */
void logSyscall(void* arg);  // NOLINT(readability-identifier-naming) Defined by 3rd Party Library.
}  //namespace logger

namespace testapprtos {

/// Index of the local Storage pointer feature of freertos containing the pointer to task object.
// static constexpr long localStoragePointerTaskObjectIndex = 0u;
BaseType_t localStoragePointerTaskObjectIndex = 0u;

#if configENABLE_CUSTOM_SVC_EXECUTER_ON_UNHANDLED
/// Memory Space keeping the Syscall_log Argument for invoking and executing out of SVC Context
static void* loggerArgument = nullptr;
#endif //configENABLE_CUSTOM_SVC_EXECUTER_ON_UNHANDLED

/// Unique defined Pointer to first Item of Linked list of OS Objects.
Task* Task::firstTask = nullptr;
/// Time since OS Started up. Used for determine runtimes
static uint64_t totalOsRuntime = 0u;

/**
 * @brief: Function Prototype for a wrapper function of the privileged Task
 * @param pArg Pointer Task Object
 */
static void taskSetupWrapper(void* pArg);

void Task::startAllTasks() {
#ifdef SYSTEMVIEW_TRACE
  SEGGER_SYSVIEW_Conf();
  SEGGER_SYSVIEW_Start();
#endif
  // Start OS
  vTaskStartScheduler();
}

Task* Task::getTaskById(TaskHandle_t id) {
  // Start by Idle Task and Search handles till found
  Task* foundTaskPtr = firstTask;
  while (foundTaskPtr->taskHandle != id) {
    if (foundTaskPtr->nextTask != nullptr) {
      foundTaskPtr = foundTaskPtr->nextTask;
    } else {
      break;
    }
  }
  return foundTaskPtr;
}

Task* Task::getTaskByName(const char* name) {
  if (nullptr == name)
    return nullptr;
  // Start by Idle Task and Search handles till found
  Task* foundTaskPtr = firstTask;
  while (strcmp(name, foundTaskPtr->_taskConfiguration.name) != 0) {
    if (foundTaskPtr->nextTask != nullptr) {
      foundTaskPtr = foundTaskPtr->nextTask;
    } else {
      break;
    }
  }
  return foundTaskPtr;
}

std::function<void(void*)> Task::getCustomSvcExecuter() {
  return _customSvcExecuter;
}

void* Task::getCustomSvcArgument() {
  return _customSvcArgument;
}

TaskHandle_t Task::getTaskHandle() {
  return taskHandle;
}

void Task::yieldFromPrivilegedContext() {
  taskYIELD();
}

TaskEventQueueReference Task::getTaskEventQueueReference() {
  return _taskEventQueue.getReference();
}

bool Task::isShutdownNotified() {
  testapprtos::TaskEventId eventId;
  return _taskEventQueue.receive(eventId, 0u) && (eventId == testapprtos::TaskEventId::Shutdown);
}

IsrControlType Task::getPendingIsrControl() {
  return _pendingIsrControl;
}

void Task::initializeTask() {
  // Create Static Task
  taskHandle = xTaskCreateStatic(taskSetupWrapper, _taskConfiguration.name, _taskConfiguration.stackSize / 4u, static_cast<void*>(this),
                                 _taskConfiguration.priority, _taskConfiguration.stackPtr, &taskControlBlock);
  // Register Task Object in TCB
  if (nullptr != taskHandle) {
    vTaskSetThreadLocalStoragePointer(taskHandle, localStoragePointerTaskObjectIndex, this);
  } else {
    // LOG_ERROR("Could not assign Task to %s", _taskConfiguration.name);
  }
}

// Default run function
void Task::run() {
  init();
  while (shouldTerminate == false) {
    process();
  }
  deinit();
  // LOG_DEBUG("Shutting down task: %s", _taskConfiguration.name);
  vTaskDelete(taskHandle);
}

// Empty functions for convenience of derived classes that don't need to
// override them
void Task::init() {}
void Task::deinit() {}
void Task::process() {}

void Task::updateRuntimeStats(uint32_t executionTime) {
  _taskRuntime += executionTime;
}

uint8_t Task::getTaskWorkload() const {
  constexpr uint8_t percentMultiplier = 100u;
  uint64_t percentTask = (_taskRuntime * percentMultiplier);
  return static_cast<uint8_t>(percentTask / totalOsRuntime);
}

#if configENABLE_CUSTOM_SVC_EXECUTER_ON_UNHANDLED
void Task::invokeTaskCustomSvc(void* pArg, const std::function<void(void*)>& syscall) {
  const uint32_t ipsr = hal::getIsrNumber();

  if (ipsr == 0u) {
    _customSvcExecuter = syscall; /* Note: we dont have to check for nullptr here, since it is checked at execution */
    _customSvcArgument = pArg;
    portBRC_PEND_CUSTOM_SVC(configTASK_INVOKED_SVC);
  } else {
    // This is already SVC/IRQ Context, simply execute
    syscall(pArg);
  }
}

/**
 * @brief Executes the given NVIC Operation
 * @param isrControl Containing NVIC Instructions
 */
void controlInterruptSyscall(IsrControlType isrControl);

void Task::controlInterrupt(uint32_t irqNumber, bool enable) {
  uint32_t ipsr = hal::getIsrNumber();
  uint32_t control = hal::getControlRegister();
  control &= 0x01u;  // Mask Control Bit

  /* Information for Pending NVIC Operation is stored in Task Object */
  _pendingIsrControl.enableCtrl = enable;
  _pendingIsrControl.irqNumber = irqNumber;

  /* Check if in Interrupt or privileged */
  if ((ipsr > 0u) or (control == 0u)) {
    /* In Interrupt the Processor is in privileged state and can execute NVIC operations */
    controlInterruptSyscall(_pendingIsrControl);
  } else {
    /* Not in Privileged State, pend a syscall dedicated to controlling the NVIC */
    portBRC_PEND_CUSTOM_SVC(configIRQCTRL_INVOKED_SVC);
  }
}

void loggerSyscallInvoker(void* argument) {
  uint32_t ipsr = hal::getIsrNumber();
  if ((ipsr == 0u) and (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)) {
    // Store parameter for SVC Execution
    loggerArgument = argument;
    portBRC_PEND_CUSTOM_SVC(configLOGGER_INVOKED_SVC);
  } else {
    // Already in SVC Context, simply execute logger syscall
    logger::logSyscall(argument);
  }
}

extern "C" void customSvcExecuterWrapper(uint32_t svCallId) {
  switch (svCallId) {
    case configIRQCTRL_INVOKED_SVC: {
      // Get Task Object
      auto* callerTask = xTaskGetCurrentTaskHandle();
      auto* taskObject = static_cast<Task*>(pvTaskGetThreadLocalStoragePointer(callerTask, localStoragePointerTaskObjectIndex));
      auto pendIsrControl = taskObject->getPendingIsrControl();
      // Execute NVIC Operation
      controlInterruptSyscall(pendIsrControl);
      break;
    }
    case configLOGGER_INVOKED_SVC: {
      logger::logSyscall(loggerArgument);
      break;
    }
    case configTASK_INVOKED_SVC: {
      // Get Calling Task Handle and grab the Task Object
      auto* callerTask = xTaskGetCurrentTaskHandle();
      auto* taskObject = static_cast<Task*>(pvTaskGetThreadLocalStoragePointer(callerTask, localStoragePointerTaskObjectIndex));
      auto svCall = taskObject->getCustomSvcExecuter();
      // Check Null and execute SVC
      if (svCall) {
        svCall(taskObject->getCustomSvcArgument());
      }
      break;
    }
    default: {
      configASSERT(svCallId);
    }
  }
}

void controlInterruptSyscall(IsrControlType isrControl) {
  if (isrControl.enableCtrl) {
    hal::enableInterrupt(isrControl.irqNumber);
  } else {
    hal::disableInterrupt(isrControl.irqNumber);
  }
}
#endif //configENABLE_CUSTOM_SVC_EXECUTER_ON_UNHANDLED

#if configENABLE_TASK_SWITCH_OUT_HOOK
extern "C" void taskSwitchedOutHook() {
  // Note: This Function is running privileged, since it is called in freeRTOS context during Task Switch
  auto* callerTask = xTaskGetCurrentTaskHandle();
  auto switchTime = hal::cpu::getCpuCycles();
  uint32_t runtime = (switchTime - taskSwitchInTime);
  // Idle Task has no Task Object and is not tracked, so do not update Runtime stats
  if (callerTask != xTaskGetIdleTaskHandle()) {
    auto* taskObject = static_cast<Task*>(pvTaskGetThreadLocalStoragePointer(callerTask, localStoragePointerTaskObjectIndex));
    taskObject->updateRuntimeStats(runtime);
  }
  taskSwitchInTime = switchTime;
  totalOsRuntime += runtime;

#ifdef SYSTEMVIEW_TRACE
  SEGGER_SYSVIEW_OnTaskStopExec();
#endif
}
#endif //configENABLE_TASK_SWITCH_OUT_HOOK

static void taskSetupWrapper(void* pArg) {
  /* The parameter value is expected to be 1 as 1 is passed in the
  pArg value in the call to xTaskCreateStatic(). */
  // configASSERT( ( uint32_t ) pArg == 1UL );

  auto* taskObject = static_cast<TaskSetup*>(pArg);

  // Run the Task
  taskObject->run();

  //Log Error
  // LOG_ERROR("Run Function returned.");
}

#if (configSUPPORT_STATIC_ALLOCATION == 1)
/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide
 an implementation of vApplicationGetIdleTaskMemory() to provide the memory that
 is used by the Idle task. */

/** Definition of Idle task stack size in bytes */
constexpr uint32_t idleTaskStackSize = 1024u;
constexpr uint32_t taskStackSize = 2048u;
StackType_t idleStack[ idleTaskStackSize ];

/** Task Control Block of Idle Task*/
static StaticTask_t idleTaskTcb;
/** Stack of Idle Task */
// STACK_DEF(idleStack, idleTaskStackSize, common);

extern "C" void vApplicationIdleHook() {}

extern "C" void vApplicationGetIdleTaskMemory(
    StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer,
    uint32_t* pulIdleTaskStackSize) {
  *ppxIdleTaskTCBBuffer = &idleTaskTcb;
  *ppxIdleTaskStackBuffer = idleStack;
  *pulIdleTaskStackSize = (idleTaskStackSize / 4u);
}
#endif

}  // namespace testapprtos
