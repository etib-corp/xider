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

#include "guillaume/renderer.hpp"
#include "guillaume/systems/button_interaction_system.hpp"

#include "guillaume/components/mouse_button_interaction.hpp"

#include <utility/event/mouse_button_event.hpp>

namespace guillaume::systems
{

	/**
	 * @brief System handling mouse button interactions.
	 */
	class MouseButton:
		public ButtonInteractionSystem<
			utility::event::MouseButtonEvent,
			components::MouseButtonInteraction,
			std::function<utility::graphic::RayF(
				const utility::event::MouseButtonEvent &)>>
	{
		public:
		/**
		 * @brief Construct the MouseButton system and subscribe to mouse button
		 * events.
		 * @param eventBus Reference to the event bus for subscribing to events.
		 * @param renderer Reference to the renderer for visual feedback.
		 */
		MouseButton(event::EventBus &eventBus, Renderer &renderer)
			: ButtonInteractionSystem<
				  utility::event::MouseButtonEvent,
				  components::MouseButtonInteraction,
				  std::function<utility::graphic::RayF(
					  const utility::event::MouseButtonEvent &)>>(
				  eventBus,
				  [&renderer
				  ](const utility::event::MouseButtonEvent &event) {
					  return renderer.getView().viewPointToRay(
						  event.getPosition());
				  })
		{
		}

		/**
		 * @brief Default destructor for the MouseButton system.
		 */
		~MouseButton(void) = default;
	};

} // namespace guillaume::systems
