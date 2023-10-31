/**
 * @file test/CommunicationChannelInterface/CommunicationChannelInterfaceTest.cpp
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

#include <CommunicationChannelInterface/TccInterface.hpp>
#include <gtest/gtest.h>
#include <string>
#include <ostream>

class CommunicationChanneTest : public testing::Test
{
public:
  tccinterface::ErrCode errCode = tccinterface::ErrCode::ERRCODE_UNDEFINED;
  tccinterface::TccInterface *tccInterfaceInstance = nullptr;
  
  virtual void SetUp()
  {
    tccInterfaceInstance = tccinterface::tccInterface;
  }

  virtual void TearDown()
  {
    tccInterfaceInstance = nullptr;
    errCode = tccinterface::ErrCode::ERRCODE_UNDEFINED;
  }
};

TEST_F(CommunicationChanneTest, init_and_shutdown)
{
  // initialize communication channel
  errCode = tccInterfaceInstance->init();
  EXPECT_EQ(tccinterface::ErrCode::SUCCESS, errCode);

  // Re-initialize communication channel -> should fail with errorCode
  errCode = tccInterfaceInstance->init();
  EXPECT_EQ(tccinterface::ErrCode::ERRCODE_TEST_CCHANNEL_ALREADY_INITIALIZED, errCode);

  // shutdown
  errCode = tccInterfaceInstance->shutdown();
  EXPECT_EQ(tccinterface::ErrCode::SUCCESS, errCode);

  // shutdown again
  errCode = tccInterfaceInstance->shutdown();
  EXPECT_EQ(tccinterface::ErrCode::ERRCODE_TEST_CCHANNEL_UNINITIALIZED, errCode);
}

TEST_F(CommunicationChanneTest, send)
{
  char buf[512u*15u] = "message to be sent to ITF";
  // initialize communication channel
  errCode = tccInterfaceInstance->init();
  EXPECT_EQ(tccinterface::ErrCode::SUCCESS, errCode);

  // send data
  errCode = tccInterfaceInstance->send(buf, sizeof(buf));
  EXPECT_EQ(tccinterface::ErrCode::SUCCESS, errCode);

  // send data failure
  char buf2[512u*16u] = "message to be sent to ITF too long";
  errCode = tccInterfaceInstance->send(buf2, sizeof(buf2));
  EXPECT_EQ(tccinterface::ErrCode::ERRCODE_TEST_CCHANNEL_SEND_FAILURE, errCode);

  // receive data
  char receiveBuf[30];
  size_t length = 0;
  errCode = tccInterfaceInstance->receive(&length, &receiveBuf, 256);
  EXPECT_EQ(tccinterface::ErrCode::SUCCESS, errCode);

  // shutdown
  errCode = tccInterfaceInstance->shutdown();
  EXPECT_EQ(tccinterface::ErrCode::SUCCESS, errCode);

  // send data
  errCode = tccInterfaceInstance->send(buf, sizeof(buf));
  EXPECT_EQ(tccinterface::ErrCode::ERRCODE_TEST_CCHANNEL_UNINITIALIZED, errCode);

  // receive data
  errCode = tccInterfaceInstance->receive(&length, &receiveBuf, 256);
  EXPECT_EQ(tccinterface::ErrCode::ERRCODE_TEST_CCHANNEL_UNINITIALIZED, errCode);
}
