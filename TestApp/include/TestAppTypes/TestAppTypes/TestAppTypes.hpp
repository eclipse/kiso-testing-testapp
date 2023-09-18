/**
 * @file include/TestAppMessage/TestAppMessage/TestAppMessage.hpp
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

#ifndef TESTAPPTYPES_HPP
#define TESTAPPTYPES_HPP

#include <cstdint>
#include <cstddef>
#include <array>
#include <cstdio>
namespace testapp {

using TestIDType = size_t;

constexpr uint8_t messagebufferSize = 245u;  /**< maximum payload size - 2 bytes for tlv-element type and length */
constexpr uint8_t maxAssertStringLength = 255u;

enum class RetCode {
  RETCODE_SUCCESS = 0,           // NOLINT(readability-identifier-naming) invalid case style for enum constant
  RETCODE_FAILURE,               /**< An unspecified failure has occurred e.g. error in third party code */ //NOLINT
  RETCODE_OUT_OF_RESOURCES,      /**< System resources have been exhausted e.g. not enough RAM */ //NOLINT
  RETCODE_INVALID_PARAM,         /*NOLINT*/ /**< One or more passed input parameters was invalid 
                                        (e.g. passed a null pointing buffer) */
  RETCODE_NOT_SUPPORTED,         /**< The requested operation is not supported by the current implementation */ //NOLINT
  RETCODE_INCONSISTENT_STATE,    /**< The module is in a state preventing it from executing the requested operation*/ //NOLINT
  RETCODE_UNINITIALIZED,         /**< The module has not been initialized */ //NOLINT
  RETCODE_NULL_POINTER,          /**< During function execution a null pointer has been encountered */ //NOLINT
  RETCODE_UNEXPECTED_BEHAVIOR,   /**< The function has produced an unintended side effect*/ //NOLINT
  RETCODE_DOUBLE_INITIALIZATION, /**< Reinitializing the module may produce inconsistency */ //NOLINT
  RETCODE_TIMEOUT,               /**< A timeout has occurred while waiting for the operation to complete */ //NOLINT
  RETCODE_TIMER_ERROR, //NOLINT
  RETCODE_SEMAPHORE_ERROR, //NOLINT
  RETCODE_QUEUE_ERROR, //NOLINT
  RETCODE_INVALID_SIGNAL_IN_ISR, //NOLINT
  RETCODE_NULLPOINTER, //NOLINT
  RETCODE_LASTELEMENT //NOLINT
};

enum class MessageType : uint8_t{
  MESSAGE_INFO = 0, //NOLINT
  MESSAGE_DEBUG, //NOLINT
  MESSAGE_WARNING, //NOLINT
  MESSAGE_ERROR, //NOLINT
  MESSAGE_CRITICAL, //NOLINT
  MESSAGE_LOG, //NOLINT
} ;
struct LogMessageT
{
  MessageType messageType{};
  std::array<uint8_t, messagebufferSize> text{};
};

/// custom error handeling
enum class ErrCode : uint8_t {
  SUCCESS = 0, //NOLINT
  FAILURE, //NOLINT
  ERRCODE_TEST_SUITE_UNKNOWN, //NOLINT
  ERRCODE_TEST_CASE_UNKNOWN, //NOLINT
  ERRCODE_TEST_SUITE_ALREADY_REGISTERED, //NOLINT
  ERRCODE_TEST_CASE_ALREADY_REGISTERED, //NOLINT
  ERRCODE_CCHANNEL_NOT_SPECIFIED, //NOLINT
  ERRCODE_TEST_CCHANNEL_INITIALISATION_FAILED, //NOLINT
  ERRCODE_TEST_CCHANNEL_RESPONSE_FAILURE, //NOLINT
  ERRCODE_MSG_INCOMPLETE, //NOLINT
  ERRCODE_MSG_INCORRECT_RESPONSE, //NOLINT
  ERRCODE_MSG_CRC_MISMATCH, //NOLINT
  ERRCODE_VERSION_MISMATCH, //NOLINT
  ERRCODE_TLVELEMENT_NOT_FOUND, //NOLINT
  ERRCODE_REPORT_TIMEOUT, //NOLINT
  ERRCODE_UNDEFINED //NOLINT
};

}  // namespace testapp

#endif
