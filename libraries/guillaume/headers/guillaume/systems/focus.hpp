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

#include <optional>

#include "guillaume/ecs/system_filler.hpp"
#include "guillaume/event/event_bus.hpp"

#include "guillaume/components/focus.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"

#include <utility/graphic/ray.hpp>

namespace guillaume::systems
{

	/**
	 * @brief System managing focus state of UI entities.
	 *
	 * The Focus system tracks which entity currently has focus and manages
	 * focus transitions between entities. It triggers focus handlers when
	 * focus is gained or lost.
	 *
	 * @see components::Focus
	 */
	class Focus:
		public ecs::SystemFiller<components::Focus, components::Transform,
								 components::Bound>
	{
		private:
		std::optional<ecs::Entity::Identifier>
			_focusedEntity;	  ///< Currently focused entity identifier

		/**
		 * @brief Check if a ray intersects with an entity's bounds.
		 * @param entityIdentifier The entity to check.
		 * @param ray The ray to test for intersection.
		 * @return True if the ray intersects the entity's bounds.
		 */
		bool isEntityIntersecting(
			const ecs::Entity::Identifier &entityIdentifier,
			const utility::graphic::RayF &ray);

		public:
		/**
		 * @brief Default constructor for the Focus system.
		 * @param eventBus The event bus for focus events.
		 */
		Focus(event::EventBus &eventBus);

		/**
		 * @brief Default destructor for the Focus system.
		 */
		~Focus(void) = default;

		/**
		 * @brief Set focus to a specific entity.
		 * @param entityIdentifier The entity to focus.
		 * @param ray The ray used to determine focus (e.g., from mouse or hand).
		 */
		void setFocus(const ecs::Entity::Identifier &entityIdentifier,
					  const utility::graphic::RayF &ray);

		/**
		 * @brief Clear the current focus.
		 */
		void clearFocus(void);

		/**
		 * @brief Get the currently focused entity.
		 * @return The focused entity identifier, or nullopt if no entity has
		 * focus.
		 */
		std::optional<ecs::Entity::Identifier> getFocusedEntity(void) const;

		/**
		 * @brief Update the Focus system for the specified entity.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
