/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#pragma once

#include <stdexcept>
#include <tuple>
#include <memory>

#include <utility/event/event.hpp>

#include "guillaume/event/event_subscriber.hpp"

namespace guillaume::event
{
	/**
	 * @brief Base class providing typed event subscription and last-event
	 * storage.
	 *
	 * EventManager is a variadic template that manages subscriptions to
	 * multiple event types and stores the last received event for each type.
	 * Systems that need to respond to events can inherit from EventManager to
	 * automatically register subscribers for specified event types.
	 *
	 * @tparam EventTypes... The event types to subscribe to. Must satisfy
	 * utility::event::InheritFromEvent constraint.
	 *
	 * @note This is a header-only template class. No instantiation of
	 * subscribers occurs until EventManager is instantiated by a derived class.
	 *
	 * @see EventSubscriber
	 * @see EventBus
	 * @see MouseInteraction
	 * @see HandInteraction
	 *
	 * @example
	 * @code
	 * class MySystem : public EventManager<MouseButtonEvent, MouseMotionEvent>
	 * {
	 *   public:
	 *     MySystem(EventBus &eventBus) : EventManager(eventBus) { }
	 *     void processEvents()
	 *     {
	 *         consumeNextEvent<MouseButtonEvent>();
	 *         auto *lastButtonEvent = getLastEvent<MouseButtonEvent>();
	 *         if (lastButtonEvent) {
	 *             // Process the last button event...
	 *         }
	 *     }
	 * };
	 * @endcode
	 */
	template<utility::event::InheritFromEvent... EventTypes> class EventManager
	{
		private:
		EventBus &_eventBus;	///< Reference to the event bus for managing
								///< event subscriptions and dispatching

		std::tuple<std::unique_ptr<EventSubscriber<EventTypes>>...>
			_subscribers;	 ///< Typed subscribers stored as a tuple for O(1)
							 ///< access by type

		std::tuple<std::unique_ptr<EventTypes>...>
			_lastEvents;	///< Stores the
							///< most recent event for each
							///< type, enabling queries
							///< without polling subscribers

		protected:
		/**
		 * @brief Consume the next pending event of a given type and store it.
		 *
		 * This method processes only the next event in the queue for the
		 * specified type and stores it for retrieval. If no pending events
		 * exist, the previously stored event remains unchanged.
		 * @tparam EventType The event type to consume and store.
		 *
		 * @example
		 * @code
		 * void update()
		 * {
		 *     consumeNextEvent<MouseButtonEvent>();
		 *     auto *lastButton = getLastEvent<MouseButtonEvent>();
		 *     if (lastButton) {
		 *         // Process button event...
		 *     }
		 * }
		 * @endcode
		 */
		template<utility::event::InheritFromEvent EventType>
		void consumeNextEvent(void)
		{
			auto &subscriber =
				*std::get<std::unique_ptr<EventSubscriber<EventType>>>(
					_subscribers);
			if (subscriber.hasPendingEvents()) {
				std::get<std::unique_ptr<EventType>>(_lastEvents) =
					subscriber.getNextEvent();
			}
		}

		/**
		 * @brief Retrieve the most recent event of a given type, if available.
		 *
		 * Returns a pointer to the last event stored for a type, or nullptr if
		 * no event has been stored yet. Call consumeNextEvent() only after
		 * using the event content.
		 *
		 * @tparam EventType The event type to retrieve.
		 * @return Pointer to the last stored event, or nullptr if none exists.
		 *
		 * @note The returned pointer remains valid until consumeNextEvent is
		 * called for the same type, or until EventManager is destroyed.
		 *
		 * @example
		 * @code
		 * auto *motionEvent = getLastEvent<MouseMotionEvent>();
		 * if (motionEvent) {
		 *     float x = motionEvent->getX();
		 *     float y = motionEvent->getY();
		 *     consumeNextEvent<MouseMotionEvent>();
		 * }
		 * @endcode
		 */
		template<utility::event::InheritFromEvent EventType>
		EventType *getLastEvent(void)
		{
			auto &ptr = std::get<std::unique_ptr<EventType>>(_lastEvents);
			return ptr ? ptr.get() : nullptr;
		}

		public:
		/**
		 * @brief Construct the EventManager and register subscribers for all
		 * event types.
		 *
		 * The constructor initializes the subscriber tuple and registers each
		 * subscriber with the provided event bus. This must be called by
		 * derived classes in their initializer list.
		 *
		 * @param eventBus The event bus to subscribe to.
		 *
		 * @example
		 * @code
		 * class MySystem : public EventManager<MouseButtonEvent,
		 * MouseMotionEvent>
		 * {
		 *   public:
		 *     MySystem(EventBus &bus) : EventManager(bus) { }
		 * };
		 * @endcode
		 */
		EventManager(EventBus &eventBus)
			: _eventBus(eventBus)
			, _subscribers(
				  std::make_unique<EventSubscriber<EventTypes>>(_eventBus)...)
		{
		}

		/**
		 * @brief Virtual destructor for proper cleanup of derived classes.
		 */
		virtual ~EventManager(void) = default;
	};
}	 // namespace guillaume::event