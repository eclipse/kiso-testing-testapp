/**
 * @file include/Tier1competetion/Tier1competetion/Task.hpp
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

#ifndef TIER1COMPETETION_TASK_HPP
#define TIER1COMPETETION_TASK_HPP

#include <Tier1competetion/TaskInterface.hpp>
#include <Tier1competetion/Tier1competetion.hpp>
// #include <TestAppRtosInterface/ConfigureTasks.hpp>
// #include <OsExtension/TaskWakeUp.hpp>
#include <TestAppRtos/Rtos.hpp>
#include <etl/map.h>
#include <etl/vector.h>
#include <utility>

/**
 * @brief: tier1competetion namespace.
 */
namespace tier1competetion {
/**
 * @brief Tier2competetion class
 */
class Task : public testapprtos::TaskSetup, public TaskInterface {
public:
  /**
   * @brief Constructor
   * @param[in] taskConfiguration Task configuration
   */
  explicit Task(const testapprtos::TaskConfiguration &taskConfiguration);

  /**
   * @brief Register MessageBufferReference from Dashboard to Tier1competetion.
   * @param[in] messageBufferReference Reference to the message buffer.
   */
  void registerDashboardToTier1competetionMessageBufferReference(
      osextension::MessageBufferReference &messageBufferReference) override;

  /**
   * @brief Register MessageBufferReference from Tier1competetion to Dashboard.
   * @param[in] messageBufferReference Reference to the message buffer.
   */
  void registerTier1competetionToDashboardMessageBufferReference(
      osextension::MessageBufferReference &messageBufferReference) override;

  /**
   * @brief Run function.
   */
  void run() override;

  /**
   * @brief Initialize.
   */
  /// @copydoc Task::initializeTask()
  void initialize() override { initializeTask(); }

private:
  Task() = delete;              /**< Remove empty (default) constructor. */
  Task(const Task &) = delete;  /**< Remove copy Constructor. */
  Task(const Task &&) = delete; /**< Remove assign Constructor. */
  const Task &operator=(const Task &) =
      delete; /**< Remove assign Operation @return NULL. */
  const Task &
  operator=(Task &&) = delete;       /**< Remove move Operation @return NULL. */
  Tier1competetion tier1competetion; /**< Tier1competetion instance. */
  /**
   * @brief Semaphore to signal, that the message buffer from TestRunnter to
   * Tier1competetion was registered.
   */
  osextension::Semaphore _dashboardToTier1competetionMessageBufferAdded{1, 0};
  /**
   * @brief Semaphore to signal, that the message buffer from Tier1competetion
   * to TestRunnter was registered.
   */
  osextension::Semaphore _tier1competetionToDashboardMessageBufferAdded{1, 0};
};

} // namespace tier1competetion

#endif // TIER1COMPETETION_TASK_HPP
