/**
 * @file Platform/${PROJECT_PLATFORM}/src/Core/Main.cpp
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
#include <TestAppRtos/Rtos.hpp>
#include <TestRunner/TaskInterface.hpp>
#include <TestEnvironment/TaskInterface.hpp>
#include <TPC/TaskInterface.hpp>
#include <SEGGER_RTT.h>

 
/**
 * @brief  Entrance point to C application
 * @return integer 0 upon exit success
 */
int main() {
  SEGGER_RTT_Init();
  // Initialize Tasks
  testrunner::taskInterface.initialize();
  testenvironment::taskInterface.initialize();
  tpcprotocol::taskInterface.initialize();

  // FreeRTOS setup and start
  testapprtos::Task::startAllTasks();

  // Endless loop. This should never be reached.
  while (true) {
    assert(false);
  }

  return 0;
}
