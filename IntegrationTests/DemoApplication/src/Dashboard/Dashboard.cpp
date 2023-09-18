/**
 * @file src/Dashboard/Dashboard/Dashboard.cpp
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

#include <Dashboard/Dashboard.hpp>

namespace dashboard
{
  /// set dashboard message Buffer references
  void Dashboard::setDashboardToTier2competetionMessageBufferReference(osextension::MessageBufferReference& mbf) {
    _dashboardToTier2competetionMessageBufferReference = &mbf;
  }
  void Dashboard::setTier2competetionToDashboardMessageBufferReference(osextension::MessageBufferReference& mbf) {
    _tier2competetionToDashboardMessageBufferReference = &mbf;
  }
  void Dashboard::setDashboardToTier1competetionMessageBufferReference(osextension::MessageBufferReference& mbf) {
    _dashboardToTier1competetionMessageBufferReference = &mbf;
  }
  void Dashboard::setTier1competetionToDashboardMessageBufferReference(osextension::MessageBufferReference& mbf) {
    _tier1competetionToDashboardMessageBufferReference = &mbf;
  }

  /// dashboard queue set setter and getter
  void Dashboard::setDashboardQueueSetHandle(QueueSetHandle_t queueSetHandle) {
    _queueSetHandle = queueSetHandle;
  }
  QueueSetHandle_t Dashboard::getDashboardQueueSetHandle() const {
    return _queueSetHandle;
  }
} // namespace testrunner

