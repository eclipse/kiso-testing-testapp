/**
 * @file include/Tier1competetion/Tier1competetion/Tier1competetion.hpp
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

#ifndef TIER1COMPETETION_TIER1COMPETETION_HPP
#define TIER1COMPETETION_TIER1COMPETETION_HPP

#include <OsExtension/MessageBuffer.hpp>
#include <DemoappTypes/DemoappTypes.hpp>
/**
 * @brief: tier1competetion namespace.
 */
namespace tier1competetion {

/**
 * @brief: Tier1competetion class.
 */
class Tier1competetion {
public:
  /**
   * @brief Run one cycle.
   */
  void runOneCycle();

  /**
   * @brief Register MessageBufferReference from Dashboard to Tier1competetion.
   * @param[in] messageBufferReference Reference to the message buffer.
   */
  void registerDashboardToTier1competetionMessageBufferReference(osextension::MessageBufferReference& messageBufferReference);

  /**
   * @brief Register MessageBufferReference from Tier1competetion To Dashboard.
   * @param[in] messageBufferReference Reference to the message buffer.
   */
  void registerTier1competetionToDashboardMessageBufferReference(osextension::MessageBufferReference& messageBufferReference);
    
  /**
   * @brief onecycle method is invoked in run method of task mechanism
   *
   */
    void oneCycle();

    void StartRace(const demoapptypes::Race& l_race);
private:
  /**
   * @brief MessageBufferReference from Dashboard to Tier1competetion.
   */
  osextension::MessageBufferReference* _dashboardToTier1competetionMessageBufferReference = nullptr;

  /**
   * @brief MessageBufferReference from Tier1competetion to Dashboard.
   */
  osextension::MessageBufferReference* _tier1competetionToDashboardMessageBufferReference = nullptr;

  std::array<demoapptypes::Athlete, demoapptypes::LANES> currentRace;
  demoapptypes::Round currentRound{};
  uint8_t results = 0;
  // demoapptypes::Race currentRace; 
  const uint8_t tier = 1;
};

}  // namespace tier1competetion

#endif  // TIER1COMPETETION_TIER1COMPETETION_HPP
