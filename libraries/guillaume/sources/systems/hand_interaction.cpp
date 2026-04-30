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

#include "guillaume/systems/hand_interaction.hpp"

namespace guillaume::systems
{

	void HandInteraction::processHover(
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		auto *lastEvent = getLastEvent<utility::event::HandMotionEvent>();
		if (lastEvent == nullptr) {
			return;
		}

		auto &interaction =
			getComponent<components::HandInteraction>(entityIdentifier);

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

	void HandInteraction::processButtonClick(
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		auto *lastEvent = getLastEvent<utility::event::HandButtonEvent>();
		if (lastEvent == nullptr) {
			return;
		}

		auto &interaction =
			getComponent<components::HandInteraction>(entityIdentifier);

		const auto button	 = lastEvent->getButton();
		const bool isPressed = lastEvent->isClicked();

		if (isPressed && isInside) {
			interaction.setClicked(button, true);
			const auto &onClickHandler =
				interaction.getOnClickHandlers().at(button);
			if (onClickHandler) {
				onClickHandler();
			}
			return;
		}

		if (!isPressed && interaction.isClicked(button) && isInside) {
			interaction.setClicked(button, false);
			const auto &onReleaseHandler =
				interaction.getOnClickReleaseHandlers().at(button);
			if (onReleaseHandler) {
				onReleaseHandler();
			}
			return;
		}

		if (!isPressed) {
			interaction.setClicked(button, false);
		}
	}

	void HandInteraction::processPinch(
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		auto *lastEvent = getLastEvent<utility::event::HandPinchEvent>();
		if (lastEvent == nullptr) {
			return;
		}

		auto &interaction =
			getComponent<components::HandInteraction>(entityIdentifier);

		if (isInside) {
			interaction.setPinched(true);
			const auto onPinch = interaction.getOnPinchHandler();
			if (onPinch) {
				onPinch();
			}
		} else {
			interaction.setPinched(false);
		}
	}

	void HandInteraction::processPoke(
		const ecs::Entity::Identifier &entityIdentifier, bool isInside)
	{
		auto *lastEvent = getLastEvent<utility::event::HandPokeEvent>();
		if (lastEvent == nullptr) {
			return;
		}

		auto &interaction =
			getComponent<components::HandInteraction>(entityIdentifier);

		if (isInside) {
			interaction.setPoked(true);
			const auto onPoke = interaction.getOnPokeHandler();
			if (onPoke) {
				onPoke();
			}
		} else {
			interaction.setPoked(false);
		}
	}

	HandInteraction::HandInteraction(event::EventBus &eventBus,
									 Renderer &renderer)
		: ecs::SystemFiller<components::HandInteraction, components::Transform,
							components::Bound>(ecs::Phase::Event)
		, event::EventManager<
			  utility::event::HandButtonEvent, utility::event::HandMotionEvent,
			  utility::event::HandPinchEvent, utility::event::HandPokeEvent>(
			  eventBus)
		, _renderer(renderer)
	{
	}

	void
		HandInteraction::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug("Updating HandInteraction system for entity "
						  + std::to_string(entityIdentifier));

		if (!requireComponent<components::HandInteraction>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)) {
			return;
		}

		bool isInside = false;

		processHover(entityIdentifier, isInside);

		processButtonClick(entityIdentifier, isInside);

		processPinch(entityIdentifier, isInside);

		processPoke(entityIdentifier, isInside);

		// Consume events only after they have been used
		consumeNextEvent<utility::event::HandButtonEvent>();
		consumeNextEvent<utility::event::HandMotionEvent>();
		consumeNextEvent<utility::event::HandPinchEvent>();
		consumeNextEvent<utility::event::HandPokeEvent>();
	}

}	 // namespace guillaume::systems
