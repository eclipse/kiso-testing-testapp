/**
 * @file include/TestFactory/TestFactory/TestFactoryBase.hpp
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

#ifndef TEST_FACTORY_TESTFACTORYBASE_HPP
#define TEST_FACTORY_TESTFACTORYBASE_HPP

#include <TestAppTypes/TestAppTypes.hpp>
#include <etl/unordered_map.h>

constexpr uint8_t maxFactoryCapacity = 10u;

/**
 * @brief: A Generic nested Factory Class
 * @tparam Parent dummy parent factory class, just will be used to categorize
 * the factories based on the Parent and Interface
 * @tparam CommonInterface refers to the common abstract class from which
 * the containing registered class have been inherited from
 */
template<typename Parent, typename CommonInterface>
class TestFactoryBase {

  /// Makes the factory register method accessible by any FactoryRegistrar class
  template<class Interface, class Derived, class Factory, testapp::TestIDType ID>
  friend class TestFactoryRegistrar;

  /// Type definition for the createInstance method provided by any to be registered class
  using CreateInstanceCallable = CommonInterface*(*)(void *memoryLocation, size_t memorySize);
  /// Type definition for the map of registered classes in factory
  using RegisteredDescendentsMap = etl::unordered_map<testapp::TestIDType, CreateInstanceCallable, maxFactoryCapacity>;

protected:
  /// Creates a possible instance of a registered class in factory at desired memory address
  static CommonInterface* createObject(testapp::TestIDType descendentID, void *memoryLocation, size_t memorySize) {
    auto it = registeredDescendents().find(descendentID);
    if (it != registeredDescendents().end()) {
      return it->second(memoryLocation, memorySize);
    }
    return nullptr;
  }

  /// Returns the reference of map of registered classes in factory
  static RegisteredDescendentsMap& registeredDescendents(){
    static RegisteredDescendentsMap registeredDescendents{};
    return registeredDescendents;
  }

  /// Registers a class in factory by desired id and its createInstance method pointer
  static bool registerClass(testapp::TestIDType desiredID, CreateInstanceCallable createInstanceFunction) {
	bool ret = false;
	if(registeredDescendents().available()) {
      auto it = registeredDescendents().find(desiredID);
      if(it == registeredDescendents().end()) {
        registeredDescendents()[desiredID] = createInstanceFunction;
        ret = true;
      }
	}
    return ret;
  }
};

#endif
