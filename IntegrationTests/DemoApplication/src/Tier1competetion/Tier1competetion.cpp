/**
 * @file src/Tier1competetion/Tier1competetion.cpp
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

#include "Tier1competetion/TaskInterface.hpp"
#include <Tier1competetion/Tier1competetion.hpp>


/**
 * @brief: tier1competetion namespace.
 */
namespace tier1competetion {

void Tier1competetion::runOneCycle() {
}

void Tier1competetion::registerDashboardToTier1competetionMessageBufferReference(osextension::MessageBufferReference& messageBufferReference) {
  _dashboardToTier1competetionMessageBufferReference = &messageBufferReference;
}

void Tier1competetion::registerTier1competetionToDashboardMessageBufferReference(osextension::MessageBufferReference& messageBufferReference) {
  _tier1competetionToDashboardMessageBufferReference = &messageBufferReference;
}

void Tier1competetion::oneCycle() { 
  if(results =! 0)
  {
    demoapptypes::CompetetionMessageT messageToDashboard{demoapptypes::Tier::TierA, currentRound, currentRace};
    _tier1competetionToDashboardMessageBufferReference->send(static_cast<void*>(&messageToDashboard), sizeof(messageToDashboard), portMAX_DELAY);
    results = 0;
  }
  
}

void Tier1competetion::StartRace(const demoapptypes::Race& l_race){
  std::vector<demoapptypes::Athlete> vec = l_race.getAthletes();
  std::copy_n(l_race.getAthletes().begin(), demoapptypes::LANES, currentRace.begin());
  currentRound.laps = l_race.round.laps;
  currentRound.style = l_race.round.style;
  results = 1;  
}
}  // namespace tier1competetion
