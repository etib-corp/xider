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
#include "guillaume/components/mouse_interaction.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/renderer.hpp"

#include <utility/event/mouse_button_event.hpp>
#include <utility/event/mouse_motion_event.hpp>

namespace guillaume::systems
{

	/**
	 * @brief System handling mouse interactions (hover and click).
	 *
	 * MouseInteraction processes mouse button and motion events from the event
	 * bus and updates MouseInteraction components for all entities. It
	 * determines whether entities are hovered or clicked based on mouse
	 * position and button state, calling registered handlers at appropriate
	 * times.
	 *
	 * This system inherits from both SystemFiller and EventManager:
	 * - **SystemFiller** provides iteration over entities with MouseInteraction
	 *   components during the Event phase.
	 * - **EventManager<MouseButtonEvent, MouseMotionEvent>** automatically
	 *   registers subscribers to those event types and provides access via
	 *   getSubscriber(), allowing efficient polling of the latest mouse state.
	 *
	 * The system maintains the last received button and motion events, which
	 * can be queried via getLastEvent() to determine current mouse state
	 * without re-processing every event.
	 *
	 * @see components::MouseInteraction - Component storing hover/click state
	 * @see HandInteraction - Counterpart system for hand-tracked interactions
	 * @see EventManager - Template base class managing event subscriptions
	 *
	 * @note Requires Transform and Bound components on entities to perform
	 * bounds checking against viewport coordinates.
	 */
	class MouseInteraction:
		public ecs::SystemFiller<components::MouseInteraction,
								 components::Transform, components::Bound>,
		public event::EventManager<utility::event::MouseButtonEvent,
								   utility::event::MouseMotionEvent>
	{
		private:
		Renderer &_renderer;	///< Reference to the renderer for view and
								///< viewport information; used for bounds
								///< checking against viewport

		/**
		 * @brief Process hover state for an entity based on mouse motion.
		 *
		 * Checks if the pointer is inside the entity bounds and updates the
		 * isHovered state, calling onHover/onUnhover handlers as needed.
		 *
		 * @param entityIdentifier The identifier of the entity to update.
		 * @param isInside Whether the mouse pointer is currently inside the
		 * entity bounds (accounting for accessibility margin).
		 */
		void processHover(const ecs::Entity::Identifier &entityIdentifier,
						  bool isInside);

		/**
		 * @brief Process click state for an entity based on button events.
		 *
		 * Updates button pressed/released state and calls the registered
		 * onClickHandler or onClickReleaseHandler based on button state and
		 * whether the pointer is inside bounds.
		 *
		 * @param entityIdentifier The identifier of the entity to update.
		 * @param isInside Whether the pointer is inside the entity bounds.
		 */
		void processButtonClick(const ecs::Entity::Identifier &entityIdentifier,
								bool isInside);

		public:
		/**
		 * @brief Construct a MouseInteraction system.
		 *
		 * Registers subscribers for MouseButtonEvent and MouseMotionEvent with
		 * the event bus via EventManager base class.
		 *
		 * @param eventBus The event bus to subscribe to.
		 * @param renderer The renderer instance for retrieving viewport bounds.
		 */
		MouseInteraction(event::EventBus &eventBus, Renderer &renderer);

		/**
		 * @brief Destructor.
		 */
		~MouseInteraction(void) = default;

		/**
		 * @brief Update mouse interaction state for an entity.
		 *
		 * For each entity with a MouseInteraction component:
		 * 1. Consumes pending events from the subscribers
		 * 2. Stores the most recent button and motion events
		 * 3. Determines if entity is hovered/inside bounds
		 * 4. Processes hover and button click state changes
		 *
		 * Called once per entity during the Event phase.
		 *
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
