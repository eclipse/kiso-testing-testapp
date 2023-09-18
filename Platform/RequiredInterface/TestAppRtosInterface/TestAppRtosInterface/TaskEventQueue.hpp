/**
 * @file Platform/RequiredInterface/TestAppRtosInterface/TestAppRtosInterface/TaskEventQueue.hpp
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

#ifndef TESTAPPRTOSINTERFACE_TASKEVENTQUEUE_HPP
#define TESTAPPRTOSINTERFACE_TASKEVENTQUEUE_HPP

#include <cstdint>
#include <OsExtension/Queue.hpp>

namespace testapprtos {

/// Task event identifier
enum class TaskEventId : uint8_t {
  Shutdown
};

/// Maximum number of events that can simultaneously occur
constexpr uint8_t maximumNumberOfEvents = 1;

/// Queue for task events
using TaskEventQueue = osextension::Queue<TaskEventId, maximumNumberOfEvents>;

/// Reference to queue for task events
using TaskEventQueueReference = osextension::QueueReference<TaskEventId>;

}  // namespace testapprtos

#endif /* TESTAPPRTOSINTERFACE_TASKEVENTQUEUE_HPP */
