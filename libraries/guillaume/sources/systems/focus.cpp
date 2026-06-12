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

#include "guillaume/systems/focus.hpp"

namespace guillaume::systems
{

	Focus::Focus(event::EventBus &eventBus)
		: ecs::SystemFiller<components::Focus, components::Transform,
							components::Bound>(ecs::Phase::Event)
		, _focusedEntity(std::nullopt)
	{
		getLogger().info() << "Focus system initialized";
	}

	bool Focus::isEntityIntersecting(
		const ecs::Entity::Identifier &entityIdentifier,
		const utility::graphic::RayF &ray)
	{
		if (!hasComponent<components::Transform>(entityIdentifier)
			|| !hasComponent<components::Bound>(entityIdentifier)) {
			return false;
		}

		auto &transform = getComponent<components::Transform>(entityIdentifier);
		auto &bound = getComponent<components::Bound>(entityIdentifier);

		const auto size = utility::math::Vector2UI(
			{ bound.getWidth(), bound.getHeight() });
		return ray.intersectRectangle(transform.getPose(), size);
	}

	void Focus::setFocus(const ecs::Entity::Identifier &entityIdentifier,
						 const utility::graphic::RayF &ray)
	{
		if (!isEntityIntersecting(entityIdentifier, ray)) {
			getLogger().debug()
				<< "Cannot set focus to entity " << entityIdentifier
				<< ": ray does not intersect";
			return;
		}

		// Clear focus from previous entity
		if (_focusedEntity.has_value()
			&& _focusedEntity.value() != entityIdentifier) {
			auto previousEntity = _focusedEntity.value();
			if (hasComponent<components::Focus>(previousEntity)) {
				auto &previousFocus =
					getComponent<components::Focus>(previousEntity);
				auto handler = previousFocus.getOnFocusLostHandler();
				if (handler) {
					getLogger().debug()
						<< "Triggering focus lost handler for entity "
						<< previousEntity;
					handler();
				}
			}
		}

		// Set focus to new entity
		_focusedEntity = entityIdentifier;
		getLogger().info() << "Focus set to entity " << entityIdentifier;

		// Trigger focus handler for new entity
		if (hasComponent<components::Focus>(entityIdentifier)) {
			auto &focus = getComponent<components::Focus>(entityIdentifier);
			auto handler = focus.getOnFocusGainedHandler();
			if (handler) {
				getLogger().debug()
					<< "Triggering focus gained handler for entity "
					<< entityIdentifier;
				handler();
			}
		}
	}

	void Focus::clearFocus(void)
	{
		if (_focusedEntity.has_value()) {
			auto previousEntity = _focusedEntity.value();
			if (hasComponent<components::Focus>(previousEntity)) {
				auto &previousFocus =
					getComponent<components::Focus>(previousEntity);
				auto handler = previousFocus.getOnFocusLostHandler();
				if (handler) {
					getLogger().debug()
						<< "Triggering focus lost handler for entity "
						<< previousEntity;
					handler();
				}
			}
			getLogger().info() << "Focus cleared from entity " << previousEntity;
			_focusedEntity = std::nullopt;
		}
	}

	std::optional<ecs::Entity::Identifier> Focus::getFocusedEntity(void) const
	{
		return _focusedEntity;
	}

	void Focus::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug() << "Updating Focus system for entity "
							<< entityIdentifier;
		// Focus management is handled through setFocus/clearFocus methods
		// This update method can be extended for automatic focus detection
	}

}	 // namespace guillaume::systems
