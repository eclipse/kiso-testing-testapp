/**
 * @file Platform/stm32f401re/src/Middleware/include/RttChannel/RttChannel.hpp
 * @copyright Copyright 2021 Robert Bosch GmbH. All rights reserved.
 *
 * This source code  is copyright protected and proprietary
 * to Robert Bosch GmbH. Only those rights that have been
 * explicitly granted to you by Robert Bosch GmbH in written
 * form may be exercised. All other rights remain with
 * Robert Bosch GmbH.
 * 
 */

#ifndef RTTCHANNEL_RTTCHANNEL_HPP
#define RTTCHANNEL_RTTCHANNEL_HPP

#include <cstdint>
#include <cstddef>
#include <array>
#include <CommunicationChannelInterface/TccInterface.hpp>
#include <etl/array_view.h>
#include <SEGGER_RTT.h>

namespace tccinterface {
/// The size in bytes for rtt channel 1 down buffer
constexpr uint16_t channelDownBufferSize = 256u;

/// The size in bytes for rtt channel 1 up buffer
constexpr uint16_t channelUpBufferSize = 256u;

/// rtt channel Id; should be in rang [1, SEGGER_RTT_MAX_NUM_UP_BUFFERS-1] otherwise channel 1 is used
constexpr uint8_t rttChannelId = 1u;

class RttChannel : public TccInterface {
public:
  /// Constructor
  explicit RttChannel(OperationMode operationMode, TickType_t blockingTime)
      : _operationMode{operationMode}, _blockingTime{blockingTime} {}

  /// Remove copy Constructor
  RttChannel(const RttChannel&) = delete;

  /// Remove assign Constructor
  RttChannel(const RttChannel&&) = delete;

  /// Remove assign Operation
  const RttChannel& operator=(const RttChannel&) = delete;

  /// Remove move Operation
  const RttChannel& operator=(RttChannel&&) = delete;

  /**
   * @brief: Interface to initialize the coordination channel for data send and data reception
   * @return: tccinterface::ErrCode enum with possible communication channel failures
   */
  tccinterface::ErrCode init() override;

  /**
   * @brief: invokes rtt SEGGER_RTT_WriteNoLock to Store a specified number of characters in SEGGER RTT control block which is then read by the host.
   * @param[in] datastream: Variable which holds the pointer to the data to be sent
   * @param[in] dataLength: Variable to hold the dataLength to be sent out
   * @return: tccinterface::ErrCode enum with possible communication channel failures
   */
  tccinterface::ErrCode send(const void* datastream, size_t dataLength) override;

  /**
   * @brief: invokes rtt SEGGER_RTT_WriteNoLock to Store a specified number of characters in SEGGER RTT control block which is then read by the host.
   * @param[in] datastream: data to be sent in a array_view
   * @return: tccinterface::ErrCode enum with possible communication channel failures
   */
  tccinterface::ErrCode send(const etl::array_view<uint8_t>& datastream) override;

  /**
   * @brief:Data receive interface invoked to receive the data from remote. 
   * @details: The caller of this interface can invoke the interface until received data becomes '0'.
   * @param[out] length: The pointer to the variable which holds received dataLength info
   * @param[in] buf: The pointer to datastream buffer to which received data will be copied
   * @param[in] maxsize: The variable which holds the maximum size of data is to be received.
   * @return: tccinterface::ErrCode enum with possible communication channel failures
   */
  tccinterface::ErrCode receive(size_t* length, void* buf, size_t maxsize) override;

  /**
   * @brief:Data receive interface invoked to receive the data from remote. 
   * @details: The caller of this interface can invoke the interface until received data becomes '0'.
   * @param[out] length: The pointer to the variable which holds received dataLength info
   * @param[out] bufView: array_view datastream to which received data will be copied
   * @return: tccinterface::ErrCode enum with possible communication channel failures
   */
  tccinterface::ErrCode receive(size_t* length, etl::array_view<uint8_t>& bufView) override;

  /**
   * @brief: Shutdown all the channels initialized
   * @return: tccinterface::ErrCode enum with possible communication channel failures
   */
  tccinterface::ErrCode shutdown() override;

  /**
   * @brief: getter to the operation mode interrupt/polling in use
   * @return: Operation Mode for channel communication: INTERRUPT_MODE = 0, POLLING_MODE = 1; 
   */
  OperationMode getOperationMode() const override;

  /**
   * @brief: Needs to be implemented if OperationMode is set to POLLING_MODE
   * @return: Polling time in Ticks
   */
  TickType_t registerPollingPeriod() const override;

protected:
  /// buffers for RTT channel 1
  std::array<char, channelDownBufferSize> _channelDownBuffer;
  std::array<char, channelUpBufferSize> _channelUpBuffer;

private:
  /// Channnel initialization status
  bool _initialized{false};

  /// channel Id
  uint8_t _channelId{1u};

  /// INTERRUPT_MODE = 0, POLLING_MODE = 1; In case of INTERRUPT_MODE implementation a semaphore reference to TPC semaphore has to be used
  OperationMode _operationMode;

  /// Polling time in Ticks
  TickType_t _blockingTime;
};

}  // namespace tccinterface

#endif  //RTTCHANNEL_RTTCHANNEL_HPP
