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

#include "guillaume/systems/mouse_interaction.hpp"

namespace guillaume::systems
{

	void MouseInteraction::processHover(
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		auto *lastEvent = getLastEvent<utility::event::MouseMotionEvent>();
		if (lastEvent == nullptr) {
			return;
		}

		auto &interaction =
			getComponent<components::MouseInteraction>(entityIdentifier);

		if (isInside) {
			if (interaction.isHovered()) {
				return;
			}

			interaction.setHovered(true);
			const auto onHover = interaction.getOnHoverHandler();
			if (onHover) {
				onHover();
			}
			return;
		}

		if (!interaction.isHovered()) {
			return;
		}

		interaction.setHovered(false);
		if (interaction.getOnUnhoverHandler()) {
			interaction.getOnUnhoverHandler()();
		}
	}

	void MouseInteraction::processButtonClick(
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		auto *lastEvent = getLastEvent<utility::event::MouseButtonEvent>();
		if (lastEvent == nullptr) {
			return;
		}

		auto &interaction =
			getComponent<components::MouseInteraction>(entityIdentifier);

		for (std::size_t buttonIndex = 0;
			 buttonIndex < static_cast<std::size_t>(
				 utility::event::MouseButtonEvent::Button::Last);
			 ++buttonIndex) {
			if (!lastEvent->getButtonsState().test(buttonIndex)) {
				continue;
			}

			const auto button =
				static_cast<utility::event::MouseButtonEvent::Button>(
					buttonIndex);
			const bool isPressed =
				lastEvent->getButtonsState().test(buttonIndex);

			if (isPressed && isInside) {
				interaction.setClicked(button, true);
				const auto &onClickHandler =
					interaction.getOnClickHandlers().at(button);
				if (onClickHandler) {
					onClickHandler();
				}
				break;
			}

			if (!isPressed && interaction.isClicked(button) && isInside) {
				interaction.setClicked(button, false);
				const auto &onReleaseHandler =
					interaction.getOnClickReleaseHandlers().at(button);
				if (onReleaseHandler) {
					onReleaseHandler();
				}
				break;
			}

			if (!isPressed) {
				interaction.setClicked(button, false);
			}
		}
	}

	MouseInteraction::MouseInteraction(event::EventBus &eventBus,
									   Renderer &renderer)
		: ecs::SystemFiller<components::MouseInteraction, components::Transform,
							components::Bound>(ecs::Phase::Event)
		, event::EventManager<utility::event::MouseButtonEvent,
							  utility::event::MouseMotionEvent>(eventBus)
		, _renderer(renderer)
	{
	}

	void MouseInteraction::update(
		const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug("Updating MouseInteraction system for entity "
						  + std::to_string(entityIdentifier));

		if (!requireComponent<components::MouseInteraction>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)) {
			return;
		}

		bool isInside = false;

		processHover(entityIdentifier, isInside);

		processButtonClick(entityIdentifier, isInside);

		// Consume events only after they have been used
		consumeNextEvent<utility::event::MouseButtonEvent>();
		consumeNextEvent<utility::event::MouseMotionEvent>();
	}

}	 // namespace guillaume::systems
