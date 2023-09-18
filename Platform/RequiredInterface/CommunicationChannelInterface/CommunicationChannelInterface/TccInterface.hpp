/**
 * @file Platform/RequiredInterface/CommunicationChannelInterface/CommunicationChannelInterface/TccInterface.hpp
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

#ifndef COMMUNICATIONCHANNELINTERFACE_TCCINTERFACEHPP
#define COMMUNICATIONCHANNELINTERFACE_TCCINTERFACEHPP

#include <cstdint>
#include <OsExtension/Semaphore.hpp>
#include <etl/array_view.h>
#include <array>

namespace tccinterface{

/// custom error handeling
enum class ErrCode : uint8_t {
  SUCCESS = 0,
  ERRCODE_TEST_CCHANNEL_UNINITIALIZED,
  ERRCODE_TEST_CCHANNEL_INITIALISATION_FAILURE,
  ERRCODE_TEST_CCHANNEL_ALREADY_INITIALIZED,
  ERRCODE_TEST_CCHANNEL_BUSY_OR_RESPONSE_FAILURE,
  ERRCODE_TEST_CCHANNEL_DATA_SIZE_OVERLIMIT,
  ERRCODE_TEST_CCHANNEL_SEND_FAILURE,
  ERRCODE_UNDEFINED
};

/// Communication channel operation mode
enum class OperationMode : uint8_t
{
  INTERRUPT_MODE = 0,  /// Interrupt based communication between target and Host 
  POLLING_MODE = 1     /// Periodically call the required Api for communication to receive data
};

/**
 * @brief:
 * The class declaration is used to define the interface for the different Test coordination channel interface
 * It defines the methods invoked by TestApp components to send and receive
 * the data from Integration Testing framework (ITF)
 * The platform specific implementation will override the base implementation
 *
 */
class TccInterface{
public:
  /// Deleted copy constructor
  TccInterface(const TccInterface&) = delete;

  /// Deleted move constructor
  TccInterface(TccInterface&&) = delete;

  /// Deleted copy assignment operator
  TccInterface& operator=(const TccInterface&) = delete;

  /// Delete move assignment operator
  TccInterface& operator=(TccInterface&&) = delete;


  /// Test Coordination Channel Interface Initialization function
  virtual tccinterface::ErrCode init(void) = 0;

  /// Test Coordination Channel Data receive interface function
  virtual tccinterface::ErrCode receive(size_t* length, void* buf, size_t maxsize) = 0;

  /// Test Coordination Channel Data receive interface function
  virtual tccinterface::ErrCode receive(size_t* length, etl::array_view<uint8_t>& bufView) = 0;

  /// Test Coordination Channel Data send interface function
  virtual tccinterface::ErrCode send(const void* datastream, size_t datalen) = 0;

  /// Test Coordination Channel Data send interface function
  virtual tccinterface::ErrCode send(const etl::array_view<uint8_t>& datastream) = 0;
 
  /// Test Coordination Channel Interface Shutdown Interface
  virtual tccinterface::ErrCode shutdown(void) = 0;

  /**
   * @brief:
   * Provide to the TPC, which operation mode needs to be setup.
   * In case of INTERRUPT_MODE, registerSemaphoreReference() needs to be used to get the semaphore that will trigger a receive().
   * In case of POLLING_MODE, registerPollingPeriod() needs to be used to tell the TPC the period in ms receive() needs to be trigger.
   */
  virtual OperationMode getOperationMode(void) const = 0;

  /// register reference to TPC semaphore (non-pure virtual function - Derived class does not have to override it)
  /// In case of INTERRUPT_MODE this method has to be defined by the derived class
  virtual void registerSemaphoreReference(__attribute__((unused)) osextension::SemaphoreReference &TPCSemaphore) {};

  /// Needs to be implemented if OperationMode is set to POLLING_MODE (non-pure virtual function - Derived class does not have to override it)
  virtual TickType_t registerPollingPeriod() const { return 0; };

protected:
  /// Default constructor
  TccInterface() = default;

  /// Destructor
  virtual ~TccInterface() = default;

};

extern TccInterface* tccInterface;

} //namespace tccinterface

#endif //COMMUNICATIONCHANNELINTERFACE_TCCINTERFACEHPP
