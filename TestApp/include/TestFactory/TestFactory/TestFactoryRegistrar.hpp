/**
 * @file include/TestFactory/TestFactory/TestFactoryRegistrar.hpp
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

#ifndef TESTFACTORY_TESTFACTORYREGISTRAR_HPP
#define TESTFACTORY_TESTFACTORYREGISTRAR_HPP

/**
 * @brief: Factory Registrar Class
 * @tparam Interface the pure abstract class which is the base class of all registered and derived classes in the factory
 * @tparam Derived the class which is going to be registered
 * @tparam Factory the factory class in which the derived class would be registered
 */
template<class Interface, class Derived, class Factory, testapp::TestIDType ID>
class TestFactoryRegistrar {
public:
  /// A static member which its initilization will do the trick
  /// of initializing of the derived class prior to the main function
  static bool registered;
  
  /// A static method which is responsible for the creation of an 
  /// instance of the derived class using placement new in a specific
  /// memory address. it will be called by the factory class in case of need 
  static Interface* createInstance(void *memoryLocation, size_t memorySize) {
    if (sizeof(Derived) <= memorySize) {
      //NOLINTNEXTLINE [cppcoreguidelines-owning-memory] returning a newly created resource of type 'Interface *' or 'gsl::owner<>' from a function whose return type is not 'gsl::owner<>'
      return new (memoryLocation) Derived();
    }
    return nullptr;
  }
};

/// registered member initialization which will eventually do the trick of
/// derived class initilization
template<class Base, class Derived, class Factory, testapp::TestIDType ID>
bool TestFactoryRegistrar<Base, Derived, Factory, ID>::registered = 
  Factory::registerClass(ID, TestFactoryRegistrar::createInstance);

#endif
