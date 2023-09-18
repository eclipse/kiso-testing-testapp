/**
 * @file include/Observer/Observer/Observer.hpp
 *
 * @copyright Copyright 2023 Robert Bosch GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 */

#ifndef SYSTEMCOMMON_OBSERVABLE_HPP
#define SYSTEMCOMMON_OBSERVABLE_HPP

namespace observer {

/**
 * Observable implementing the generic observer design pattern for
 * single observers.
 *
 * The observer pattern allows to notify changes of this observable
 * object's state to external observers by means of event notifications.
 * Doing so event processing can be delegated to an observer which needs
 * not to be known to this observable object and so effectively decouples
 * event detection and emission from event processing.
 *
 * This low-complexity implementation supports only the registration of
 * a single observer to each observable object as opposed to most common
 * implementation as most use cases almost always require only a single
 * observer. In cases where multiple observers are needed, a dispatching
 * observer can easily be implemented.
 *
 * @tparam ObserverType Type of the observer defining the observable
 *         event types
 */
template <class ObserverType>
class Observable {
public:
  /// @return true if there is an observer attached
  bool hasObserver() {
    return _observer != nullptr;
  }

  /**
   * Set the observer to notify in case of an event
   *
   * @param observer Observing object
   */
  void setObserver(ObserverType& observer) {
    _observer = &observer;
  }

  /// Removes the current observer if any
  void removeObserver() {
    _observer = nullptr;
  }

protected:
  /// Notification handler without arguments
  void notifyObserver() {
    if (hasObserver()) {
      _observer->onNotify();
    }
  }

  /**
   * Generic notification of arbitrarily typed events
   *
   * @tparam Generic event type of the event to notify
   * @param event Event to notify
   */
  template <class Event>
  void notifyObserver(Event event) {
    if (hasObserver()) {
      _observer->onNotify(event);
    }
  }

private:
  /// observer receiving notifications (onNotify(...))
  ObserverType* _observer{nullptr};
};

/**
 * Generic observer supporting multiple event notification types
 *
 * @tparam Event1
 * @tparam Events
 */
template <typename Event1, typename... Events>
/// @cond IGNORE
class Observer : Observer<Event1>, Observer<Events...> {
  /// @endcond
public:
  /// declare onNotify from base classes for single argument
  using Observer<Event1>::onNotify;

  /// declare onNotify from base classes for multiple arguments
  using Observer<Events...>::onNotify;
};

/**
 * Generic observer for a single event notification type
 *
 * @tparam Event Event type to notify
 */
template <typename Event>
class Observer<Event> {
public:
  /**
   * Notification handler for the given event type
   *
   * Defaults to an empty implementation effectively ignoring
   * this given type of events.
   */
  virtual void onNotify(Event){};
};

/**
 * Specialized observer for no-argument notifications.
 */
template <>
class Observer<void> {
public:
  /// no-argument event notification
  virtual void onNotify(){};
};

}  // namespace observer

#endif  //SYSTEMCOMMON_OBSERVABLE_HPP
