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

#include <concepts>

#include <utility/graphic/ray.hpp>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/system_filler.hpp"
#include "guillaume/event/event_manager.hpp"
#include "guillaume/event/event_bus.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Concept to check if a type is a valid RayProvider.
	 * A RayProvider must be invocable with a const reference to an event and
	 * return a utility::graphic::RayF.
	 *
	 * @tparam T The type to check.
	 * @return True if T is a valid RayProvider, false otherwise.
	 */
	template<typename T, typename EventType>
	concept IsRayProvider = std::is_invocable_r<utility::graphic::RayF, T,
												const EventType &>::value;

	/**
	 * @brief Generic system handling button interactions for any input type.
	 *
	 * @tparam EventType The event type (must satisfy
	 * utility::event::InheritFromEvent).
	 * @tparam InteractionComponent The component type storing interaction
	 * state.
	 * @tparam RayProvider Callable that extracts a RayF from an event.
	 */
	template<utility::event::InheritFromEvent EventType,
			 ecs::InheritFromComponent InteractionComponent,
			 IsRayProvider<EventType> RayProvider>
	class ButtonInteractionSystem:
		public ecs::SystemFiller<InteractionComponent, components::Transform,
								 components::Bound>,
		public event::EventManager<EventType>
	{
		private:
		RayProvider _rayProvider;	 //< Callable to get a ray from an event

		/**
		 * @brief Checks if an event represents a pressed state.
		 * @param event The event to check.
		 * @return True if the event represents a pressed state, false
		 * otherwise.
		 */
		static bool eventIsPressed(const EventType &event)
		{
			if constexpr (requires {
							  {
								  event.isPressed()
							  } -> std::convertible_to<bool>;
						  }) {
				return event.isPressed();
			} else if constexpr (requires {
									 {
										 event.isButtonPressed()
									 } -> std::convertible_to<bool>;
								 }) {
				return event.isButtonPressed();
			} else {
				static_assert(
					sizeof(EventType) == 0,
					"Event type must provide isPressed() or isButtonPressed()");
				return false;
			}
		}

		public:
		/**
		 * @brief Construct the ButtonInteractionSystem and subscribe to events.
		 * @param eventBus Reference to the event bus for subscribing to events.
		 * @param rayProvider Callable to extract a RayF from an event.
		 */
		ButtonInteractionSystem(event::EventBus &eventBus,
								RayProvider rayProvider)
			: ecs::SystemFiller<InteractionComponent, components::Transform,
								components::Bound>(ecs::Phase::Event)
			, event::EventManager<EventType>(eventBus)
			, _rayProvider(std::move(rayProvider))
		{
		}

		/**
		 * @brief Default destructor for the ButtonInteractionSystem.
		 */
		~ButtonInteractionSystem(void) = default;

		/**
		 * @brief This function is called before call update on all entities and
		 * can be used to set up any necessary state or resources.
		 */
		void prepare(void) override
		{
			this->consumeNextEvent();
		}

		/**
		 * @brief This function is called after call update on all entities and
		 * can be used to release any resources or reset state.
		 */
		void cleanup(void) override
		{
		}

		/**
		 * @brief Update function called for each entity with the required
		 * components. Handles button interaction logic based on events.
		 * @param entityIdentifier The identifier of the entity being updated.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override
		{
			auto buttonEvent = this->getLastEvent();
			if (!buttonEvent)
				return;

			this->getLogger().error()
				<< "ButtonInteractionSystem::update: Button event received";

			const auto button = buttonEvent->getButton();
			if constexpr (requires { EventType::Button::Unknown; }) {
				if (button == EventType::Button::Unknown)
					return;
			}

			auto ray = _rayProvider(*buttonEvent);

			auto &transform =
				this->template getComponent<components::Transform>(
					entityIdentifier);
			auto &bound = this->template getComponent<components::Bound>(
				entityIdentifier);
			auto &interaction =
				this->template getComponent<InteractionComponent>(
					entityIdentifier);

			const utility::graphic::PositionF center(
				transform.getPose().getPosition().x + bound.getWidth() / 2.0f,
				transform.getPose().getPosition().y + bound.getHeight() / 2.0f,
				transform.getPose().getPosition().z);

			auto centeredPose = transform.getPose();
			centeredPose.setPosition(center);

			const auto size = utility::math::Vector2F(
				{ bound.getWidth(), bound.getHeight() });
			const bool isIntersecting =
				ray.intersectRectangle(centeredPose, size);
			const bool isPressed = eventIsPressed(*buttonEvent);

			if (isPressed && isIntersecting) {
				interaction.setButtonPressed(button, true);
				const auto &handler =
					interaction.getOnButtonPressHandler(button);
				if (handler) {
					handler();
				}
			} else if (!isPressed && interaction.isButtonPressed(button)
					   && isIntersecting) {
				interaction.setButtonPressed(button, false);
				const auto &handler =
					interaction.getOnButtonReleaseHandler(button);
				if (handler) {
					handler();
				}
			} else if (!isPressed) {
				interaction.setButtonPressed(button, false);
			}
		}
	};

}	 // namespace guillaume::systems
