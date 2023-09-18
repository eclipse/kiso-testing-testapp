/**
 * @file mock/SemaphoreMock/SemaphoreMock.hpp
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

#ifndef OSEXTENSION_SEMAPHOREMOCK_HPP
#define OSEXTENSION_SEMAPHOREMOCK_HPP

#include <OsExtension/SemaphoreInterface.hpp>
#include <OsAbstraction/OsAbstraction.hpp>

#include <cstddef>

#include <gmock/gmock.h>

namespace osextension {

class SemaphoreMock : public SemaphoreInterface {
public:
  SemaphoreMock();
  virtual ~SemaphoreMock();

  MOCK_METHOD(bool, signal, (), (override));
  MOCK_METHOD(bool, signalFromIsr, (SemaphoreTaskWokenTokenType*), (override));
  MOCK_METHOD(bool, wait, (TickType_t), (override));
  MOCK_METHOD(bool, waitFromIsr, (SemaphoreTaskWokenTokenType*), (override));
  MOCK_METHOD(bool, reset, (), (override));
  MOCK_METHOD(size_t, getCount, (), (override));
  MOCK_METHOD(size_t, getCountFromIsr, (), (override));
  MOCK_METHOD(OsExtensionHandleType, getHandleForQueueSet, (), (const override));
};

}  // namespace osextension

#endif /* OSEXTENSION_SEMAPHOREMOCK_HPP */