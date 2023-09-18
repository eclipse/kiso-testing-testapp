/**
 * @file Platform/RequiredInterface/TestAppRtosInterface/TestAppRtosInterface/Task.hpp
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

#ifndef TESTAPPRTOSINTERFACE_TASK_HPP
#define TESTAPPRTOSINTERFACE_TASK_HPP

#include <FreeRTOS.h>
#include <TestAppRtosInterface/TaskConfig.hpp>
#include <TestAppRtosInterface/TaskEventQueue.hpp>
#include <task.h>
#include <array>
#include <cstring>
#include <functional>

/**
 * Macro used in submacros to create strings
 */
#define HELPER_STRINGERIZER(reg) #reg
/**
 * Helper Macro for Stack Definition and Access
 * Usage like:
 * STACK_DEF(name, size) ... Use this where you want to setup your stack variable - The space
 * STACK(name) ... use the name you given your stack with STACK_DEF to get the access pointer
 */
#define STACK_DEF(name, size, region_name) \
  alignas(32) static StackType_t taskContextStack_##name[size / 4u] __attribute__((section(HELPER_STRINGERIZER(.task_section_##region_name##_stack)))) __attribute__((used)) /* NOLINT (cppcoreguidelines-avoid-c-arrays)  it cannot be modified because it uses Third-Party-Macros */
/**
 * Access Stack defined by STACK_DEF
 */
#define STACK(name) taskContextStack_##name


namespace testapprtos { 

/**
 * Helper Struct to communicate NVIC Operations to designated Systemcall
 */
struct IsrControlType {
  uint32_t irqNumber; /**< Corresponding NVIC Control Line */
  bool enableCtrl;    /**< Instruction */
};

/**
 * @brief:
 * The class is used as base for the two types of Tasks that can be defined by an underlying
 * OS. It contains all needed Data for static allocalisation of the Task and all its needed Resources.
 * Furthermore the classes are setup to be linked into a list of objects to
 * guarantee a consistent way of starting and initializing all Tasks.
 */
class Task {
public:
  /// Deleted default constructor
  Task() = delete;

  /// Deleted copy constructor
  Task(const Task&) = delete;

  /// Deleted move constructor
  Task(Task&&) = delete;

  /// Deleted copy assignment operator
  Task& operator=(const Task&) = delete;

  /// Delete move assignment operator
  Task& operator=(Task&&) = delete;

  /**
   * @brief: Define a Constructor for the static initialization
   * @param[in] taskConfiguration @see class rtosTaskConfiguration
   */
  explicit Task(const TaskConfiguration& taskConfiguration) : _taskConfiguration(taskConfiguration), _taskEventQueue(taskConfiguration.name) {
    // Setup a Linked List for Initialization Chain
    Task* pointerToLinkedList = firstTask;
    if (firstTask == nullptr) {
      firstTask = this;
    } else {
      while (pointerToLinkedList->nextTask != nullptr) {
        pointerToLinkedList = pointerToLinkedList->nextTask;
      }
      pointerToLinkedList->nextTask = this;
    }
  };

  /**
   * @brief: Destructor of this Object. Ends Task and removes it from execution list
   */
  virtual ~Task() {
    vTaskDelete(taskHandle);
  };

  /// One Function to start all OS related Tasks and Startup the OS
  static void startAllTasks() __attribute__((used));

  /**
   * @brief: Get Task Object by ID
   * @param[in] id Task Handle ID
   * @return Reference to Object
   */
  static Task* getTaskById(TaskHandle_t id) __attribute__((used));
  /**
   * @brief: Get Task Object by Name
   * @param[name] name Task Name
   * @return Reference to Object
   */
  static Task* getTaskByName(const char* name) __attribute__((used));

  // Virtual Functions for Deriving
  virtual void initializeTask(); /**< Creates Static Task (is called upon startup) */

#if configENABLE_MPU
  /**
   * @brief Drop Privileged Rights of Task
   */
  static void dropPrivilege();
#endif //#if configENABLE_MPU

  /**
   * @brief Update Runtime Stats
   *
   * This Function is called during a Task Switch Process.
   * It updates all relevant Timings to determine the CPU stress per Task.
   *
   * @param executionTime Time the task was run in CPU Cycles
   */
  void updateRuntimeStats(uint32_t executionTime);

  /**
   * @brief: Get Workload of Task
   * Return Collected Workload Data of task Object and associated Task.
   * @return Workload of this Task in Percent
   */
  uint8_t getTaskWorkload() const;

#if configENABLE_CUSTOM_SVC_EXECUTER_ON_UNHANDLED
  /**
   * @brief Invoke Custom SVC
   *
   * Generates a user Interrupt to run a given executer.
   * These are unique for each task.
   *
   * @param pArg Pointer to the custom argument that is given to the Executer in SVC Context
   * @param syscall Pointer to Syscall Function that should be executed on this invoke. Will be stored.
   */
  void invokeTaskCustomSvc(void* pArg, const std::function<void(void*)>& syscall);
#endif //#if configENABLE_CUSTOM_SVC_EXECUTER_ON_UNHANDLED

  /**
   * @brief Returns the currently Registered Task Custom SVC Executer
   * @return Custom SVC
   */
  std::function<void(void*)> getCustomSvcExecuter();

  /**
   * @brief Get Custom SVC Argument
   * @return Stored SVC Argument
   */
  void* getCustomSvcArgument();

  /**
   * @brief Get Task Object OS Handle
   * @return freeRTOS Task Handle
   */
  TaskHandle_t getTaskHandle();

  /**
   * @brief Yield current Task from a privileged Context.
   * Careful! This Function call may ONLY be executed in a privileged
   * Context to force a reschedule of Tasks.
   * E.G.: ISR, Privileged Task or SystCall.
   */
  static void yieldFromPrivilegedContext();

#if configENABLE_CUSTOM_SVC_EXECUTER_ON_UNHANDLED
  /**
   * @brief Control an Interrupt
   *
   * This Function is used to enable or disable a System Interrupt.
   * @param irqNumber Designated IRQ Line Number
   * @param enable true = turns line on, false = turns line off
   */
  void controlInterrupt(uint32_t irqNumber, bool enable);
#endif //#if configENABLE_CUSTOM_SVC_EXECUTER_ON_UNHANDLED

  /**
   * @brief Get reference to task event queue
   * @return Reference to task event queue
   */
  TaskEventQueueReference getTaskEventQueueReference();

  /// Default run function that handles shutdown task event
  virtual void run();

  /// Init function to be executed once after task creation
  virtual void init();

  /// Deinit function to be executed before termination
  virtual void deinit();

  /// Process function to be executed cyclically
  virtual void process();

  /**
   * @brief Evaluates whether the shutdown was triggered
   * @return True when shutdown has been triggered, false otherwise.
   */
  bool isShutdownNotified();

  /**
   * @brief Get ISR Control Field
   * ISR Pending Control can be obtained via this call.
   * Its a bitfield encoded Word to instruct SVC NVIC Operations.
   * @return pendingIsrControl
   */
  IsrControlType getPendingIsrControl();

  /// Indicates whether this task should terminate itself
  bool shouldTerminate = false;

  // First item in list of os objects
  static Task* firstTask;

  /// Next item in list of os objects
  Task* nextTask = nullptr;

protected:
  /// The task configuration
  const TaskConfiguration& _taskConfiguration;

  StaticTask_t taskControlBlock; /**< Static Control Block for task */
  TaskHandle_t taskHandle;       /**< Task Handle is Initialized upon the start of the OS */

  /// Queue of task events to be processed
  TaskEventQueue _taskEventQueue;

private:
  uint64_t _taskRuntime = 0u;                    /**< Total Time this Task has run */
  void* _customSvcArgument = nullptr;            /**< Pointer to a SVC Element that is given in new Context */
  std::function<void(void*)> _customSvcExecuter; /**< System Call Executed Function that is given customSvcArgument */
  IsrControlType _pendingIsrControl;             /**< Field for Storing Interrupt controlling Operations */
};

} //namespace testapprtos

#endif //TESTAPPRTOSINTERFACE_TASK_HPP
