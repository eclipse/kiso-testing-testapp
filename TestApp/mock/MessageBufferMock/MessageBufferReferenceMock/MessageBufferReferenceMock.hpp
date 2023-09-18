/**
 * @file mock/MessageBufferMock/MessageBufferReferenceMock/MessageBufferReferenceMock.hpp
 *
 * @copyright Copyright 2020 Robert Bosch GmbH. All rights reserved.
 *
 * This source code is copyright protected and proprietary
 * to Robert Bosch GmbH. Only those rights that have been
 * explicallRunnersIteratorly granted to you by Robert Bosch GmbH in wrallRunnersIteratorten
 * form may be exercised. All other rights remain wallRunnersIteratorh
 * Robert Bosch GmbH.
 */

#ifndef OSEXTENSION_MESSAGEBUFFERREFERENCEMOCK_HPP
#define OSEXTENSION_MESSAGEBUFFERREFERENCEMOCK_HPP

#include <OsExtension/MessageBuffer.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace osextension {

class MessageBufferReferenceMock : public MessageBufferReference {
public:
  MessageBufferReferenceMock(void);
  virtual ~MessageBufferReferenceMock();

  MOCK_METHOD(size_t, send, (const void*, size_t), (override));
  MOCK_METHOD(size_t, send, (const void*, size_t, TickType_t), (override));
  MOCK_METHOD(size_t, send, (etl::array_view<const uint8_t>), (override));
  MOCK_METHOD(size_t, send, (etl::array_view<const uint8_t>, TickType_t), (override));
  MOCK_METHOD(size_t, sendFromISR, (const void*, size_t, BaseType_t*), (override));
  MOCK_METHOD(size_t, sendFromISR, (etl::array_view<const uint8_t>, BaseType_t*), (override));
  MOCK_METHOD(size_t, receive, (void*, size_t, TickType_t), (override));
  MOCK_METHOD(size_t, receive, (etl::array_view<uint8_t>, TickType_t), (override));
  MOCK_METHOD(size_t, receiveFromISR, (void*, size_t, BaseType_t*), (override));
  MOCK_METHOD(size_t, receiveFromISR, (etl::array_view<uint8_t> data, BaseType_t*), (override));
  MOCK_METHOD(bool, reset, (), (override));
  MOCK_METHOD(bool, isEmpty, (), (override));
  MOCK_METHOD(bool, isFull, (), (override));
  MOCK_METHOD(size_t, numMessagesAvailable, (), (override));
  MOCK_METHOD(size_t, numBytesAvailable, (), (override));
  MOCK_METHOD(OsExtensionHandleType, getHandleForQueueSet, (), (const override));
  };

}  // namespace osextension

#endif /* OSEXTENSION_MESSAGEBUFFERREFERENCEMOCK_HPP */
