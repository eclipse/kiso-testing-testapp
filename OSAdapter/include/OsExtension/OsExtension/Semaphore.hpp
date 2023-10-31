/**
 * @file include/OsExtension/OsExtension/Semaphore.hpp
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 */

#ifndef OSEXTENSION_SEMAPHORE_HPP
#define OSEXTENSION_SEMAPHORE_HPP

#include <OsExtension/SemaphoreInterface.hpp>

#include <cstdint>

namespace osextension {

/**
 * @brief Reference Class for Controlling a OS Semaphore
 * Used to copy and share for a thread safe Inter-
 * task-signaling of OS Semaphore Objects
 */
class SemaphoreReference : public SemaphoreInterface {
public:
  /** Reassign native OS Handle Type for readability and portability */
  using SemaphoreHandleType = SemaphoreHandle_t;

  /// Constructor
  SemaphoreReference(void) = default;

  /// Copy constructor
  SemaphoreReference(const SemaphoreReference&) = default;

  /// Move constructor
  SemaphoreReference(SemaphoreReference&&) noexcept = default;

  /// Copy assignment operator @return
  SemaphoreReference& operator=(const SemaphoreReference&) = default;

  /// Move assignment operator @return
  SemaphoreReference& operator=(SemaphoreReference&&) noexcept = default;

  /// Destructor
  ~SemaphoreReference(void) override = default;

  /// @copydoc SemaphoreInterface::signal()
  bool signal(void) override;

  /// @copydoc SemaphoreInterface::signalFromIsr(SemaphoreTaskWokenTokenType*)
  bool signalFromIsr(SemaphoreTaskWokenTokenType* taskWoken) override;

  /// @copydoc SemaphoreInterface::wait()
  bool wait(uint32_t timeout) override;

  /// @copydoc SemaphoreInterface::waitFromIsr(SemaphoreTaskWokenTokenType*)
  bool waitFromIsr(SemaphoreTaskWokenTokenType* taskWoken) override;

  /// @copydoc SemaphoreInterface::getCount()
  size_t getCount(void) override;

  /// @copydoc SemaphoreInterface::getCount()
  size_t getCountFromIsr(void) override;

  /// @copydoc SemaphoreInterface::reset()
  bool reset(void) override;

  /**
   * @brief Returns handle to the Rtos Semaphore.
   * @return Semaphore Handle.
   */
  [[deprecated("Copy object to share references or use getHandleForQueueSet() if you need add to FreeRTOS queue set")]] SemaphoreHandleType getHandle(void) const {
    return _semaphore;
  }

  /**
   * @copydoc OsExtensionInterface::getHandleForQueueSet
   */
  OsExtensionHandleType getHandleForQueueSet(void) const override {
    return _semaphore;
  }

protected:
  /// Semaphore Handle
  SemaphoreHandleType _semaphore = {};  //NOLINT(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes) attribute should be accessible by derived classes
};

/**
 * The Memory Allocation Template Class for a Rtos Semaphore
 */
class Semaphore : public SemaphoreReference {
public:
  /** Reassign native OS Type for readability and portability */
  using StaticSemaphoreType = StaticSemaphore_t;

  /**
   * @brief Constructor
   *
   * @param[in] maxCount      Maximum Semaphore counter value
   * @param[in] initialCount  Initial Semaphore counter value
   */
  Semaphore(uint32_t maxCount, uint32_t initialCount) {
    /* Differentiate between a counting and a binary Semaphore */
    if (maxCount == 1u) {
      _semaphore = osabstraction::semaphoreCreateBinaryStatic(&_semaphoreDef);
      if (initialCount >= 1u) {
        /* Check if Binary Semaphore is initially open */
        osabstraction::semaphoreGive(_semaphore);
      }
    } else {
      _semaphore = osabstraction::semaphoreCreateCountingStatic(maxCount, initialCount, &_semaphoreDef);
    }
  }

  /// Deleted copy constructor
  Semaphore(const Semaphore&) = delete;

  /// Deleted move constructor
  Semaphore(Semaphore&&) = delete;

  /// Delete copy assignment operator @return
  Semaphore& operator=(const Semaphore&) = delete;

  /// Deleted move assignment operator @return
  Semaphore& operator=(Semaphore&&) = delete;

  /// Destructor
  ~Semaphore(void) override {
    osabstraction::semaphoreDelete(this->_semaphore);
  }

  /**
   * @brief Constructs and returns a Semaphore Reference to this Object
   * @return Semaphore Reference Object
   */
  SemaphoreReference getReference(void) const {
    return SemaphoreReference(*this);  //NOLINT(cppcoreguidelines-slicing) That is the wanted point here.
  }

private:
  StaticSemaphoreType _semaphoreDef = {}; /**< Static Semaphore Definition */
};

}  // namespace osextension

#endif  // OSEXTENSION_SEMAPHORE_HPP
