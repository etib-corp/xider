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

#include "guillaume/systems/button_interaction_system.hpp"

#include "guillaume/components/hand_button_interaction.hpp"

#include <utility/event/hand_button_event.hpp>

namespace guillaume::systems
{

	/**
	 * @brief System handling hand button interactions.
	 */
	class HandButton:
		public ButtonInteractionSystem<
			utility::event::HandButtonEvent,
			components::HandButtonInteraction,
			std::function<utility::graphic::RayF(
				const utility::event::HandButtonEvent &)>>
	{
		public:
		/**
		 * @brief Construct the HandButton system and subscribe to hand button
		 * events.
		 * @param eventBus Reference to the event bus for subscribing to events.
		 */
		HandButton(event::EventBus &eventBus)
			: ButtonInteractionSystem<
				  utility::event::HandButtonEvent,
				  components::HandButtonInteraction,
				  std::function<utility::graphic::RayF(
					  const utility::event::HandButtonEvent &)>>(
				  eventBus,
				  [](const utility::event::HandButtonEvent &event) {
					  return event.getPose().toForwardRay();
				  })
		{
		}

		/**
		 * @brief Default destructor for the HandButton system.
		 */
		~HandButton(void) = default;
	};

} // namespace guillaume::systems
