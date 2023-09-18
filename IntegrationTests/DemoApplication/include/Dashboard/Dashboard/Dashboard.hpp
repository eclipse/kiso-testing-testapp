/**
 * @file include/Dashboard/Dashboard/Dashboard.hpp
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

#ifndef DASHBOARD_DASHBOARD_HPP
#define DASHBOARD_DASHBOARD_HPP

#include <OsExtension/MessageBuffer.hpp>

namespace dashboard {

class Dashboard {
public:
  /// Constructor.
  Dashboard() = default;

  /// Default destructor
  ~Dashboard() = default;

  /// Copy constructor.
  Dashboard(Dashboard const&) = delete;

  /// Move constructor.
  Dashboard(Dashboard&&) = delete;

  /// @brief Copy assignment operator.
  /// @return result of copy assignment
  Dashboard& operator=(Dashboard const&) = delete;

  /// @brief Move assignment operator.
  /// @return result of move assignment
  Dashboard& operator=(Dashboard&&) = delete;
  

  /**
   * @brief one cycle Method to be run by TestRuner task
   */
  void oneCycle(QueueSetMemberHandle_t& queueSetMemberHandle);

  /**
   * @brief set _dashboardToTier2competetionMessageBuffer Reference
   */
  void setDashboardToTier2competetionMessageBufferReference(osextension::MessageBufferReference& mbf);

  /**
   * @brief set _tier2competetionToDashboardMessageBuffer Reference
   */
  void setTier2competetionToDashboardMessageBufferReference(osextension::MessageBufferReference& mbf);

  /**
   * @brief set _dashboardToTier1competetionMessageBuffer Reference
   */
  void setDashboardToTier1competetionMessageBufferReference(osextension::MessageBufferReference& mbf);

  /**
   * @brief set _tier1competetionToDashboardMessageBuffer Reference
   */
  void setTier1competetionToDashboardMessageBufferReference(osextension::MessageBufferReference& mbf);

  /**
   * @brief set dashboard queue set handle
   */
  void setDashboardQueueSetHandle(QueueSetHandle_t queueSetHandle);

  /**
   * @brief get dashboard queue set handle
   */
  QueueSetHandle_t getDashboardQueueSetHandle() const;

private:
  /// Message Buffer reference for incoming messages from Tier1competetion
  osextension::MessageBufferReference* _tier1competetionToDashboardMessageBufferReference = nullptr;

  /// Message Buffer reference for incoming messages from Tier2competetion
  osextension::MessageBufferReference* _tier2competetionToDashboardMessageBufferReference = nullptr;

  /// Outgoing Message Buffer reference to the Tier1competetion
  osextension::MessageBufferReference* _dashboardToTier1competetionMessageBufferReference = nullptr;

  /// Outgoing Message Buffer reference to the Tier2competetion
  osextension::MessageBufferReference* _dashboardToTier2competetionMessageBufferReference = nullptr;

  /// FreeRTOS queue set handle
  QueueSetHandle_t _queueSetHandle = nullptr;
};

}  // namespace dashboard

#endif // DASHBOARD_DASHBOARD_HPP
