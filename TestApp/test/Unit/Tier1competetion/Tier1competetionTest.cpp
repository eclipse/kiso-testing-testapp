/**
 * @file test/MessageHandler/MessageHandlerTest.cpp
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

#include <DemoappTypes/DemoappTypes.hpp>
#include <Tier1competetion/Tier1competetion.hpp>

#include <FreeRtosMock.hpp>
#include <MessageBufferReferenceMock/MessageBufferReferenceMock.hpp>
#include <OsAbstraction/OsAbstraction.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::A;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;
// Test fixture class
class Tier1competetionTest : public testing::Test {
public:
  osextension::MessageBufferReferenceMock
      Tier1competetionToDashboardMessageBuffer;

  tier1competetion::Tier1competetion tier1competetion{};
  demoapptypes::CompetetionMessageT dataSentToDashboard{};

  virtual void SetUp() {
    freertos::mock = std::make_unique<freertos::Mock>();
    tier1competetion.registerTier1competetionToDashboardMessageBufferReference(
        Tier1competetionToDashboardMessageBuffer);
  }

  virtual void TearDown() { freertos::mock.reset(nullptr); }
};

TEST_F(Tier1competetionTest, processTestCmdTestSuiteSetup) {
  std::vector<demoapptypes::Athlete> vec;
  demoapptypes::Race race(vec);
  race += demoapptypes::Athlete{"Athlete1", 21.56, 0};
  race += demoapptypes::Athlete{"Athlete2", 21.42, 0};
  race += demoapptypes::Athlete{"Athlete3", 21.52, 0};
  race += demoapptypes::Athlete{"Athlete4", 21.15, 0};
  race += demoapptypes::Athlete{"Athlete5", 21.52, 0};
  race += demoapptypes::Athlete{"Athlete6", 21.25, 0};
  race += demoapptypes::Athlete{"Athlete7", 21.22, 0};
  race += demoapptypes::Athlete{"Athlete8", 21.13, 0};
  race.round = {demoapptypes::Laps::_50M, demoapptypes::Style::FREESTYLE};

  std::copy_n(race.getAthletes().begin(), demoapptypes::LANES,
              dataSentToDashboard.todashboard.begin());
  dataSentToDashboard.competerionTier = demoapptypes::Tier::TierA;
  dataSentToDashboard.round = {demoapptypes::Laps::_50M,
                               demoapptypes::Style::FREESTYLE};

  EXPECT_CALL(Tier1competetionToDashboardMessageBuffer,
              send(A<const void *>(), _, _))
      .Times(1)
      .WillOnce(Return(sizeof(dataSentToDashboard)));

  tier1competetion.StartRace(race);
  tier1competetion.oneCycle();
}

TEST(Test1, Tier1competetionTest2) { EXPECT_TRUE(1); }
