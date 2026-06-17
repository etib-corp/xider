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
							components::Bound,
							components::MouseButtonInteraction,
							components::HandButtonInteraction,
							components::HandPinchInteraction,
							components::HandPokeInteraction>(ecs::Phase::Event)
		, _focusedEntity(std::nullopt)
	{
		getLogger().info() << "Focus system initialized";
	}

	void Focus::setFocus(const ecs::Entity::Identifier &entityIdentifier)
	{
		// Save the current focused entity as last focused before changing
		if (_focusedEntity.has_value()
			&& _focusedEntity.value() != entityIdentifier) {
			_lastFocusedEntity = _focusedEntity.value();
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
			auto &focus	 = getComponent<components::Focus>(entityIdentifier);
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
			// Save the current focused entity as last focused before clearing
			_lastFocusedEntity = _focusedEntity.value();

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
			getLogger().info()
				<< "Focus cleared from entity " << previousEntity;
			_focusedEntity = std::nullopt;
		}
	}

	std::optional<ecs::Entity::Identifier> Focus::getFocusedEntity(void) const
	{
		return _focusedEntity;
	}

	std::optional<ecs::Entity::Identifier>
		Focus::getLastFocusedEntity(void) const
	{
		return _lastFocusedEntity;
	}

	void Focus::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug()
			<< "Updating Focus system for entity " << entityIdentifier;

		// Check if entity was clicked with mouse
		if (hasComponent<components::MouseButtonInteraction>(
				entityIdentifier)) {
			auto &mouseInteraction =
				getComponent<components::MouseButtonInteraction>(
					entityIdentifier);
			if (mouseInteraction.isButtonPressed(
					utility::event::MouseButtonEvent::Button::Left)) {
				setFocus(entityIdentifier);
				return;
			}
		}

		// Check if entity was clicked with hand button
		if (hasComponent<components::HandButtonInteraction>(entityIdentifier)) {
			auto &handInteraction =
				getComponent<components::HandButtonInteraction>(
					entityIdentifier);
			// Check common hand buttons (A, B, X, Y)
			if (handInteraction.isButtonPressed(
					utility::event::HandButtonEvent::Button::A)
				|| handInteraction.isButtonPressed(
					utility::event::HandButtonEvent::Button::B)
				|| handInteraction.isButtonPressed(
					utility::event::HandButtonEvent::Button::X)
				|| handInteraction.isButtonPressed(
					utility::event::HandButtonEvent::Button::Y)) {
				setFocus(entityIdentifier);
				return;
			}
		}

		// Check if entity was clicked with hand pinch
		if (hasComponent<components::HandPinchInteraction>(entityIdentifier)) {
			auto &pinchInteraction =
				getComponent<components::HandPinchInteraction>(
					entityIdentifier);
			if (pinchInteraction.isPinch()) {
				setFocus(entityIdentifier);
				return;
			}
		}

		// Check if entity was clicked with hand poke
		if (hasComponent<components::HandPokeInteraction>(entityIdentifier)) {
			auto &pokeInteraction =
				getComponent<components::HandPokeInteraction>(entityIdentifier);
			if (pokeInteraction.isPoke()) {
				setFocus(entityIdentifier);
				return;
			}
		}
	}

}	 // namespace guillaume::systems
