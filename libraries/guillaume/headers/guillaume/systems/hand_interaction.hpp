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

#include "guillaume/ecs/system_filler.hpp"
#include "guillaume/event/event_manager.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/hand_interaction.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/renderer.hpp"

#include <utility/event/hand_button_event.hpp>
#include <utility/event/hand_motion_event.hpp>
#include <utility/event/hand_pinch_event.hpp>
#include <utility/event/hand_poke_event.hpp>

namespace guillaume::systems
{

	/**
	 * @brief System handling hand interactions (hover, click, pinch, poke).
	 *
	 * HandInteraction processes hand button, motion, pinch, and poke events
	 * from the event bus and updates HandInteraction components for all
	 * entities. It determines whether entities are hovered, clicked, pinched,
	 * or poked based on hand position and gesture state.
	 *
	 * This system inherits from both SystemFiller and EventManager:
	 * - **SystemFiller** provides iteration over entities with HandInteraction
	 *   components during the Event phase.
	 * - **EventManager<HandButtonEvent, HandMotionEvent, HandPinchEvent,
	 *   HandPokeEvent>** automatically registers subscribers to those event
	 * types and provides access via getSubscriber(), enabling efficient polling
	 * of the latest hand state.
	 *
	 * The system maintains the last received events for all tracked gesture
	 * types, which can be queried via getLastEvent() to determine current hand
	 * state.
	 *
	 * **Event Processing Flow:**
	 * - Consumes pending HandButtonEvent and HandMotionEvent to update hover
	 * state
	 * - Consumes pending HandPinchEvent to update pinch state
	 * - Consumes pending HandPokeEvent to update poke state
	 * - Calls registered handlers when state transitions occur
	 *
	 * @see components::HandInteraction - Component storing
	 * hover/click/pinch/poke state
	 * @see MouseInteraction - Counterpart system for mouse interactions
	 * @see EventManager - Template base class managing event subscriptions
	 *
	 * @note Requires Transform and Bound components on entities to perform
	 * bounds checking against hand position and viewport coordinates.
	 *
	 * @note Unlike MouseInteraction which processes a single click per frame,
	 * HandInteraction handles multiple discrete events (pinch, poke) that may
	 * not have persistent state.
	 */
	class HandInteraction:
		public ecs::SystemFiller<components::HandInteraction,
								 components::Transform, components::Bound>,
		public event::EventManager<
			utility::event::HandButtonEvent, utility::event::HandMotionEvent,
			utility::event::HandPinchEvent, utility::event::HandPokeEvent>
	{
		private:
		Renderer &_renderer;	///< Reference to the renderer for view and
								///< viewport information; used for bounds
								///< checking against viewport

		/**
		 * @brief Process hover state for an entity based on hand motion.
		 *
		 * Checks if the hand is inside the entity bounds and updates the
		 * isHovered state, calling onHover/onUnhover handlers as needed.
		 *
		 * @param entityIdentifier The identifier of the entity to update.
		 * @param isInside Whether the hand is currently inside the entity
		 * bounds (accounting for accessibility margin).
		 */
		void processHover(const ecs::Entity::Identifier &entityIdentifier,
						  bool isInside);

		/**
		 * @brief Process button click state for an entity based on hand button
		 * events.
		 *
		 * Updates button pressed/released state and calls the registered
		 * onClickHandler or onClickReleaseHandler based on button state and
		 * whether the hand is inside bounds.
		 *
		 * @param entityIdentifier The identifier of the entity to update.
		 * @param isInside Whether the hand is inside the entity bounds.
		 */
		void processButtonClick(const ecs::Entity::Identifier &entityIdentifier,
								bool isInside);

		/**
		 * @brief Process pinch gesture state for an entity.
		 *
		 * Updates pinch state and calls the registered onPinchHandler when a
		 * pinch gesture occurs inside the entity bounds.
		 *
		 * @param entityIdentifier The identifier of the entity to update.
		 * @param isInside Whether the hand is inside the entity bounds.
		 */
		void processPinch(const ecs::Entity::Identifier &entityIdentifier,
						  bool isInside);

		/**
		 * @brief Process poke gesture state for an entity.
		 *
		 * Updates poke state and calls the registered onPokeHandler when a
		 * poke gesture occurs inside the entity bounds.
		 *
		 * @param entityIdentifier The identifier of the entity to update.
		 * @param isInside Whether the hand is inside the entity bounds.
		 */
		void processPoke(const ecs::Entity::Identifier &entityIdentifier,
						 bool isInside);

		public:
		/**
		 * @brief Construct a HandInteraction system.
		 *
		 * Registers subscribers for HandButtonEvent, HandMotionEvent,
		 * HandPinchEvent, and HandPokeEvent with the event bus via EventManager
		 * base class.
		 *
		 * @param eventBus The event bus to subscribe to.
		 * @param renderer The renderer instance for retrieving viewport bounds.
		 */
		HandInteraction(event::EventBus &eventBus, Renderer &renderer);

		/**
		 * @brief Destructor.
		 */
		~HandInteraction(void) = default;

		/**
		 * @brief Update hand interaction state for an entity.
		 *
		 * For each entity with a HandInteraction component:
		 * 1. Consumes pending events from all subscribers
		 * 2. Stores the most recent event for each gesture type
		 * 3. Determines if entity is hovered/inside bounds based on hand
		 * position
		 * 4. Processes hover, button click, pinch, and poke state changes
		 * 5. Calls registered handlers when state transitions occur
		 *
		 * Called once per entity during the Event phase.
		 *
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
