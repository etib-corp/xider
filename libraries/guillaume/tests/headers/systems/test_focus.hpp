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

#include <gtest/gtest.h>

#include <memory>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_registry.hpp"
#include "guillaume/ecs/system_phase.hpp"

#include "guillaume/components/focus.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"

#include "guillaume/systems/focus.hpp"
#include "guillaume/event/event_bus.hpp"

#include <utility/graphic/ray.hpp>
#include <utility/graphic/pose.hpp>
#include <utility/math/vector.hpp>

namespace guillaume::systems::tests
{

	class TestFocus: public ::testing::Test
	{
		protected:
		std::unique_ptr<event::EventBus> _eventBus;
		std::unique_ptr<Focus> _focusSystem;
		ecs::ComponentRegistry _componentRegistry;
		ecs::EntityRegistry _entityRegistry;

		TestFocus(void)			  = default;
		~TestFocus(void) override = default;

		void SetUp(void) override
		{
			_eventBus	= std::make_unique<event::EventBus>();
			_focusSystem = std::make_unique<Focus>(*_eventBus);
		}

		void TearDown(void) override
		{
			_focusSystem.reset();
			_eventBus.reset();
		}

		/**
		 * @brief Create an entity with Focus, Transform, and Bound components.
		 * @return The identifier of the created entity.
		 */
		ecs::Entity::Identifier createFocusableEntity(
			const utility::graphic::PoseF &pose = utility::graphic::PoseF(),
			float width						  = 100.0f,
			float height					  = 50.0f)
		{
			auto &entity = _entityRegistry.createEntity();
			auto id	   = entity.getIdentifier();

			_componentRegistry.addComponent<components::Focus>(id);
			_componentRegistry.addComponent<components::Transform>(id);
			_componentRegistry.addComponent<components::Bound>(id);

			auto &transform = _componentRegistry.getComponent<components::Transform>(id);
			transform.setPose(pose);

			auto &bound = _componentRegistry.getComponent<components::Bound>(id);
			bound.setWidth(static_cast<unsigned int>(width));
			bound.setHeight(static_cast<unsigned int>(height));

			return id;
		}
	};

}	 // namespace guillaume::systems::tests
