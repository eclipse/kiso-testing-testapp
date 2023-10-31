/**
 * @file include/TPC/TPC/TPC.hpp
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

#ifndef TPC_HPP
#define TPC_HPP
#include <TestAppTypes/TestAppTypes.hpp>
#include <MessageHandler/MessageHandler.hpp>
#include <OsExtension/MessageBuffer.hpp>
#include <OsExtension/SemaphoreInterface.hpp>
#include <CommunicationChannelInterface/TccInterface.hpp>
#include<etl/array_view.h>

namespace tpcprotocol
{
  class TPC
  {
  public:
    /// Constructor.
    TPC() = default;

    ///default constructor
    ~TPC() = default;

    /// Copy constructor.
    TPC(TPC const &) = delete;

    /// Move constructor.
    TPC(TPC &&) = delete;

    /// @brief Copy assignment operator.
    /// @return result of copy assignment
    TPC &operator=(TPC const &) = delete;

    /// @brief Move assignment operator.
    /// @return result of move assignment
    TPC &operator=(TPC &&) = delete;

   /**
   * @brief Method to validate bytestream sent from ITF, by calculatingCRC and verifying CRC
   *
   * @return testapp::ErrCode
   */
    static bool validateITFByteStream(const etl::array_view<uint8_t>& bytestream, uint8_t length);

    /**
   * @brief Method to send Acknowledgement or Nacknowledgement back to ITF
   *
   * @return testapp::ErrCode
   */
    static testapp::RetCode sendACKorNACK(message::MessageSubType subType, uint8_t tokenID);

     /**
   * @brief Method to send TestReport to ITF using FDX_send API
   *
   * @return testapp::ErrCode
   */
    static testapp::RetCode sendReportToITF(const message::MessageStructure &reportMsg, uint8_t reportLength);

   /**
   * @brief set _TestRunnerToTPCMessageBuffer Reference
   */
    void setTPCToTestRunnerMessageBufferReference(osextension::MessageBufferReference& mbf);

     /**
   * @brief set _TestRunnerToTPCMessageBuffer Reference
   */
    void setTestRunnerTPCMessageBufferReference(osextension::MessageBufferReference& mbf);
    /**
   * @brief onecycle method is invoked in run method of task mechanism
   *
   */
    void oneCycle();

    private:
     /**
   * @brief Method to calculate CRC
   * @return uint16_t buffer
   */
    static uint16_t calculateCRC(const etl::array_view<uint8_t>& buffer, uint8_t length);

    /// Message Buffer reference for incoming messages from TestRunner
    osextension::MessageBufferReference* _testRunnerToTPCMessageBuffer = nullptr;

    /// Message Buffer reference for Outgoing messages from TestRunner
    osextension::MessageBufferReference* _tpcToTestRunnerMessageBuffer = nullptr;
  };

} // namespace tpcprotocol
#endif
