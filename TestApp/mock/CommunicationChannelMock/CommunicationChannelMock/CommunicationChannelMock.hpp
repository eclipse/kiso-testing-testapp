/**
 * @file mock/FDXModuleMock/FDXModuleMock.hpp
 *
 * @copyright Copyright 2020 Robert Bosch GmbH. All rights reserved.
 *
 * This source code is copyright protected and proprietary
 * to Robert Bosch GmbH. Only those rights that have been
 * explicallRunnersIteratorly granted to you by Robert Bosch GmbH in wrallRunnersIteratorten
 * form may be exercised. All other rights remain wallRunnersIteratorh
 * Robert Bosch GmbH.
 */

#ifndef FDXMODULEMOCK_HPP
#define FDXMODULEMOCK_HPP

#include <CommunicationChannelInterface/TccInterface.hpp>
#include <etl/array_view.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace tccinterface
{
  class TccInterfaceMock : public TccInterface
  {
  public:
    TccInterfaceMock();
    virtual ~TccInterfaceMock();
    
    MOCK_METHOD(ErrCode, init, (), (override));
    MOCK_METHOD(ErrCode, send, (const void * sendbuffer, size_t len), (override));
    MOCK_METHOD(ErrCode, send, (const etl::array_view<uint8_t>& datastream), (override));
    MOCK_METHOD(ErrCode, receive, (size_t * length, void *buf, size_t maxsize), (override));
    MOCK_METHOD(ErrCode, receive, (size_t * length, etl::array_view<uint8_t>& bufView), (override));
    MOCK_METHOD(OperationMode, getOperationMode, (), (override, const));
    MOCK_METHOD(void, registerSemaphoreReference, (osextension::SemaphoreReference& TPCSemaphore));
    MOCK_METHOD(TickType_t, registerPollingPeriod, (), (const));
    MOCK_METHOD(ErrCode, shutdown, (), (override));
  };
} // namespace tccinterface
#endif
