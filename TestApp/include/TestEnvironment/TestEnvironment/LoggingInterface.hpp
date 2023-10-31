/**
 * @file include/TestEnvironment/TestEnvironment/LoggingInterface.hpp
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
#ifndef TESTENVIRONMENT_LOGGINGINTERFACE_HPP
#define TESTENVIRONMENT_LOGGINGINTERFACE_HPP

#include <TestAppTypes/TestAppTypes.hpp>

/**
 * @brief: testenvironment namespace
 */
namespace testenvironment {
/**
 * @brief LoggingInterface
 */
class LoggingInterface {
public:
  /* Enable default constructor */
  LoggingInterface() = default;
  virtual testapp::RetCode logInfo(const char* msg) = 0;
  virtual testapp::RetCode logDebug(const char* msg) = 0;
  virtual testapp::RetCode logWarning(const char* msg) = 0;
  virtual testapp::RetCode logError(const char* msg) = 0;
  virtual testapp::RetCode logCritical(const char* msg) = 0;
  virtual testapp::RetCode logMessage(const char* msg) = 0;
  virtual void resetFailureFlag() = 0;
  virtual bool getFailureFlag() = 0;
};

}  // namespace testenvironment
#endif  // TESTENVIRONMENT_LOGGINGINTERFACE_HPP
