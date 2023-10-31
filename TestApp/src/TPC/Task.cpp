/**
 * @file src/TPC/Task.cpp
 * @brief TPC Task
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

#include <TPC/Task.hpp>
#include <TestAppTypes/TestAppTypes.hpp>
#include <MessageHandler/MessageHandler.hpp>
#include <CommunicationChannelInterface/TccInterface.hpp>
#include <cstdint>
namespace tpcprotocol
{
  /// Create the task without using any dynamic memory allocation
  constexpr uint32_t taskStackSize = 2048u;
  StackType_t xStack[taskStackSize];

  /// Task descriptor
  const testapprtos::TaskConfiguration taskConfiguration = {
      taskStackSize,
      xStack,
      "tpcprotocol",
      4u};

  /// Creation of global task object
  Task task{taskConfiguration};
  TaskInterface &taskInterface = task;

  Task::Task(const testapprtos::TaskConfiguration &taskConfiguration) : TaskSetup(taskConfiguration)
  {
    // register callbacks
    _tpc.setTPCToTestRunnerMessageBufferReference(_TPCToTestRunnerMessageBuffer);
    _tpc.setTestRunnerTPCMessageBufferReference(_TestRunnerToTPCMessageBuffer);
  }

  /**
   * @brief run function
   */
  void Task::run()
  {
    // Insert the message buffer reference to TestCoordinator
    _TestRunnerToTPCMessageBuffer = testrunner::taskInterface.getTestRunnerToTPCMessageBufferReference();

    // Register the message buffer reference of the TPCoordinator to TestRunner task
    _TPCToTestRunnerMessageBuffer = testrunner::taskInterface.getTPCToTestRunnerMessageBufferReference();

    if (tccinterface::OperationMode::INTERRUPT_MODE == tccinterface::tccInterface->getOperationMode())
      // INTERRUPT_MODE : pass reference to TPC semaphore to TCCInterface implementation
      tccinterface::tccInterface->registerSemaphoreReference(_TCCInterfacerReferencePairAdded);
    else
      // POLLING_MODE : get the polling timeout from the corresponding communication channel
      _blockingTime = tccinterface::tccInterface->registerPollingPeriod();

    tccinterface::tccInterface->init();

    while (1)
    {
      if (tccinterface::OperationMode::INTERRUPT_MODE == tccinterface::tccInterface->getOperationMode())
        _TCCInterfacerReferencePairAdded.wait(portMAX_DELAY);
      else
        vTaskDelay(_blockingTime);

      _tpc.oneCycle();
    }
  }

} // tpcprotocol
