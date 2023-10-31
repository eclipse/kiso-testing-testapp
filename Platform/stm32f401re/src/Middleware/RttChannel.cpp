/**
 * @file Platform/stm32f401re/src/Middleware/RttChannel.cpp
 * @copyright Copyright 2021 Robert Bosch GmbH. All rights reserved.
 *
 * This source code  is copyright protected and proprietary
 * to Robert Bosch GmbH. Only those rights that have been
 * explicitly granted to you by Robert Bosch GmbH in written
 * form may be exercised. All other rights remain with
 * Robert Bosch GmbH.
 * 
 */

#include <RttChannel/RttChannel.hpp>

namespace tccinterface {
/// Creation of global rttChannel object
RttChannel rttChannel(OperationMode::POLLING_MODE, pdMS_TO_TICKS(200));
TccInterface* tccInterface = &rttChannel;

OperationMode RttChannel::getOperationMode() const {
  return _operationMode;
}

TickType_t RttChannel::registerPollingPeriod() const {
  return _blockingTime;
}

tccinterface::ErrCode RttChannel::init() {
  const char* channelName = "TestApp";
  // use rtt channel 1 if the channel specified is channel 0 or out of rang
  _channelId = (rttChannelId == 0u || rttChannelId > SEGGER_RTT_MAX_NUM_UP_BUFFERS - 1) ? 1u : rttChannelId;

  SEGGER_RTT_ConfigUpBuffer(_channelId, channelName, &_channelUpBuffer[0], channelUpBufferSize, SEGGER_RTT_MODE_DEFAULT);
  SEGGER_RTT_ConfigDownBuffer(_channelId, channelName, &_channelDownBuffer[0], channelDownBufferSize, SEGGER_RTT_MODE_DEFAULT);
  _initialized = true;

  return (tccinterface::ErrCode::SUCCESS);
}

tccinterface::ErrCode RttChannel::send(const void* dataStream, size_t dataLength) {
  tccinterface::ErrCode errCode = tccinterface::ErrCode::ERRCODE_TEST_CCHANNEL_SEND_FAILURE;

  if (_initialized) {
    auto count = SEGGER_RTT_WriteNoLock(_channelId, dataStream, dataLength);
    if (count == dataLength) {
      errCode = tccinterface::ErrCode::SUCCESS;
    }
  }
  return errCode;
}

tccinterface::ErrCode RttChannel::send(const etl::array_view<uint8_t>& datastream) {
  return send(static_cast<const void*>(datastream.data()), datastream.size());
}

tccinterface::ErrCode RttChannel::receive(size_t* length, void* buf, size_t maxsize) {
  tccinterface::ErrCode errCode = tccinterface::ErrCode::ERRCODE_TEST_CCHANNEL_BUSY_OR_RESPONSE_FAILURE;

  if (_initialized && (nullptr != length)) {
    *length = static_cast<uint32_t>(SEGGER_RTT_Read(_channelId, buf, maxsize));
    errCode = tccinterface::ErrCode::SUCCESS;
  }
  return errCode;
}

tccinterface::ErrCode RttChannel::receive(size_t* length, etl::array_view<uint8_t>& bufView) {
  return receive(length, static_cast<void*>(bufView.data()), bufView.size());
}

tccinterface::ErrCode RttChannel::shutdown() {
  _initialized = false;
  return (tccinterface::ErrCode::SUCCESS);
}
}  // namespace tccinterface
