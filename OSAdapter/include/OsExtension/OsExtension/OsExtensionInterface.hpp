/**
 * @file include/OsExtension/OsExtension/OsExtensionInterface.hpp
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 */

#ifndef OSEXTENSION_OSEXTENSIONINTERFACE_HPP
#define OSEXTENSION_OSEXTENSIONINTERFACE_HPP

#include <OsAbstraction/OsAbstraction.hpp>

namespace osextension {

/**
 * @brief Interface for all OsExtension modules to implement
 */
class OsExtensionInterface {
public:
  /** Reassign native OS type for readability and portability */
  using OsExtensionHandleType = QueueSetMemberHandle_t;

  /// Constructor
  OsExtensionInterface() = default;

  /// Copy constructor
  OsExtensionInterface(OsExtensionInterface const&) = default;

  /// Move constructor
  OsExtensionInterface(OsExtensionInterface&&) noexcept = default;

  /// Copy assignment operator @return
  OsExtensionInterface& operator=(OsExtensionInterface const&) = default;

  /// Move assignment operator @return
  OsExtensionInterface& operator=(OsExtensionInterface&&) noexcept = default;

  /// Destructor
  virtual ~OsExtensionInterface() = default;

  /**
   * @brief Gets a handle for use with FreeRTOS QueueSets
   *
   * @return handle
   */
  virtual OsExtensionHandleType getHandleForQueueSet(void) const = 0;
};

}  // namespace osextension

#endif /* OSEXTENSION_OSEXTENSIONINTERFACE_HPP */
