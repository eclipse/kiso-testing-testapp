/**
 * @file include/Tier2competetion/Tier2competetion/Tier2competetion.hpp
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

#ifndef TIER2COMPETETION_HPP
#define Tier2competetion_HPP
#include <OsExtension/MessageBuffer.hpp>
#include <OsExtension/SemaphoreInterface.hpp>
#include<etl/array_view.h>

namespace tier2competetion
{
  class Tier2competetion
  {
  public:
    /// Constructor.
    Tier2competetion() = default;

    ///default constructor
    ~Tier2competetion() = default;

    /// Copy constructor.
    Tier2competetion(Tier2competetion const &) = delete;

    /// Move constructor.
    Tier2competetion(Tier2competetion &&) = delete;

    /// @brief Copy assignment operator.
    /// @return result of copy assignment
    Tier2competetion &operator=(Tier2competetion const &) = delete;

    /// @brief Move assignment operator.
    /// @return result of move assignment
    Tier2competetion &operator=(Tier2competetion &&) = delete;

   /**
   * @brief set _DashboardToTier2competetionMessageBuffer Reference
   */
    void setTier2competetionToDashboardMessageBufferReference(osextension::MessageBufferReference& mbf);

     /**
   * @brief set _DashboardToTier2competetionMessageBuffer Reference
   */
    void setDashboardTier2competetionMessageBufferReference(osextension::MessageBufferReference& mbf);
    /**
   * @brief onecycle method is invoked in run method of task mechanism
   *
   */
    void oneCycle();

    private:
    /// Message Buffer reference for incoming messages from Dashboard
    osextension::MessageBufferReference* _dashboardToTier2competetionMessageBuffer = nullptr;

    /// Message Buffer reference for Outgoing messages from Dashboard
    osextension::MessageBufferReference* _tier2competetionToDashboardMessageBuffer = nullptr;
  };

} // namespace tier2competetion
#endif
