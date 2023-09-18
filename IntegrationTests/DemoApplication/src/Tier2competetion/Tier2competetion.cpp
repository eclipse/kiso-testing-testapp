/**
 * @file src/Tier2competetion/Tier2competetion/Tier2competetion.cpp
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 */

#include "Tier2competetion/Tier2competetion.hpp"
#include "Tier2competetion/TaskInterface.hpp"
#include <cstdint>
#include <array>
#include <etl/array_view.h>

namespace tier2competetion
{
  static bool receiveNextMessage = true;

   /// set tPC message Buffer references outgoing
  void Tier2competetion::setTier2competetionToDashboardMessageBufferReference(osextension::MessageBufferReference& mbf) {
    _tier2competetionToDashboardMessageBuffer = &mbf;
  }

   /// set tPC message Buffer references incoming
  void Tier2competetion::setDashboardTier2competetionMessageBufferReference(osextension::MessageBufferReference& mbf) {
    _dashboardToTier2competetionMessageBuffer = &mbf;
  }

  void Tier2competetion::oneCycle() { 
  }

} // namespace tpcprotocol
