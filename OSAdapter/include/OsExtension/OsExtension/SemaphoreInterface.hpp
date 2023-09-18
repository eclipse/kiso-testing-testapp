/**
 * @file include/OsExtension/OsExtension/SemaphoreInterface.hpp
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 */

#ifndef OSEXTENSION_SEMAPHOREINTERFACE_HPP
#define OSEXTENSION_SEMAPHOREINTERFACE_HPP

#include <OsAbstraction/OsAbstraction.hpp>
#include <OsExtension/OsExtensionInterface.hpp>
#include <cstdint>

namespace osextension {

/**
 * @brief Interface to a Semaphore.
 */
class SemaphoreInterface : public OsExtensionInterface {
public:
  /** Reassign native OS Type for readability and portability */
  using SemaphoreTaskWokenTokenType = BaseType_t;

  /// Constructor
  SemaphoreInterface(void) = default;

  /// Copy constructor
  SemaphoreInterface(const SemaphoreInterface&) = default;

  /// Move constructor
  SemaphoreInterface(SemaphoreInterface&&) noexcept = default;

  /// Copy assignment operator @return
  SemaphoreInterface& operator=(const SemaphoreInterface&) = default;

  /// Move assignment operator @return
  SemaphoreInterface& operator=(SemaphoreInterface&&) noexcept = default;

  /// Destructor
  ~SemaphoreInterface(void) override = default;

  /**
   * @brief Signals a Semaphore to give resources.
   *
   * @retval true Semaphore was signaled and resource given.
   * @retval false Semaphore is not taken by anyone or maximum count is reached.
   */
  virtual bool signal(void) = 0;

  /**
   * @brief Signals a Semaphore to give resources from an ISR.
   *
   * @param[out] taskWoken  Pointer to variable containing a bool expression
   * 						 whether or not a task was woken by this call.
   *
   * @retval true Semaphore was signaled and resource given.
   * @retval false Semaphore is not taken by anyone or maximum count is reached.
   */
  virtual bool signalFromIsr(SemaphoreTaskWokenTokenType* taskWoken) = 0;

  /**
   * @brief Waits for this Semaphore Object resource.
   *
   * @param[in] timeout Time the task waits for Semaphore
   *
   * @retval true Resource is allocated and open for Task
   * @retval false Timeout occurred
   */
  virtual bool wait(uint32_t timeout) = 0;

  /**
   * @brief Waits for this Semaphore Object resource within ISR Context.
   * Note: There is no blocking in ISR, so if the resource is not available this
   * call will return immediately.
   *
   * @param[out] taskWoken 	Pointer to variable containing a bool expression
   * 						 whether or not a task was woken by this call.
   *
   * @retval true Resource is allocated and open for Task
   * @retval false Resource not availiable.
   */
  virtual bool waitFromIsr(SemaphoreTaskWokenTokenType* taskWoken) = 0;

  /**
   * @brief Returns count as integer of Semaphore Object.
   *
   * If semaphore is a counting semaphore then the semaphores
   * current count is returned. If the semaphore is a binary semaphore
   * then 1 is returned if the semaphore is available, and 0 is returned if the semaphore is blocked.
   *
   * @return count between 0 and N, 0 if blocked
   */
  virtual size_t getCount(void) = 0;

  /**
   * @brief Returns count as integer of Semaphore Object from ISR.
   *
   * If semaphore is a counting semaphore then the semaphores
   * current count is returned. If the semaphore is a binary semaphore
   * then 1 is returned if the semaphore is available, and 0 is returned if the semaphore is blocked.
   *
   * @return count between 0 and N, 0 if blocked
   */
  virtual size_t getCountFromIsr(void) = 0;

  /**
   * @brief Resets the given Semaphore to its initial state.
   *
   * @retval true Semaphore is reset.
   * @retval false Resource is blocked or not available anymore
   */
  virtual bool reset(void) = 0;
};

}  // namespace osextension

#endif  // OSEXTENSION_SEMAPHORE_HPP
