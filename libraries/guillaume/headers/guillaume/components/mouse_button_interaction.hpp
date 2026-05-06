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

#include <functional>
#include <map>

#include <utility/event/mouse_button_event.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component handling mouse button interactions.
	 * @see systems::MouseButton
	 */
	class MouseButtonInteraction: public ecs::Component
	{
		public:
		static constexpr utility::event::MouseButtonEvent::Button Buttons[] = {
			utility::event::MouseButtonEvent::Button::Left,
			utility::event::MouseButtonEvent::Button::Middle,
			utility::event::MouseButtonEvent::Button::Right,
			utility::event::MouseButtonEvent::Button::X1,
			utility::event::MouseButtonEvent::Button::X2
		};	  ///< Array of mouse buttons to facilitate iteration over button
			  ///< states and handlers

		public:
		using ButtonPressHandler =
			std::function<void(void)>;	  ///< Type for button press handlers
		using ButtonReleaseHandler =
			std::function<void(void)>;	  ///< Type for button release handlers

		private:
		std::map<utility::event::MouseButtonEvent::Button, ButtonPressHandler>
			_onButtonPressHandlers {};	  ///< Map of button press handlers for
										  ///< each button
		std::map<utility::event::MouseButtonEvent::Button, ButtonReleaseHandler>
			_onButtonReleaseHandlers {};	///< Map of button release handlers
											///< for each button
		std::map<utility::event::MouseButtonEvent::Button, bool>
			_isButtonPressed {};	///< Map of button press states for each
									///< button

		public:
		/**
		 * @brief Construct a MouseButtonInteraction component with default
		 * handlers and states.
		 */
		MouseButtonInteraction(void);

		/**
		 * @brief Default destructor for MouseButtonInteraction.
		 */
		~MouseButtonInteraction(void) = default;

		/**
		 * @brief Set the handler for a button press event.
		 * @param button The mouse button to set the handler for.
		 * @param handler The function to call when the specified button is
		 * pressed.
		 * @return Reference to this MouseButtonInteraction for method chaining.
		 */
		MouseButtonInteraction &setOnButtonPressHandler(
			const utility::event::MouseButtonEvent::Button &button,
			const ButtonPressHandler &handler);

		/**
		 * @brief Get the handlers for mouse button press events.
		 * @return A map of mouse buttons to their corresponding press
		 * handlers.
		 */
		const ButtonPressHandler &getOnButtonPressHandler(
			const utility::event::MouseButtonEvent::Button &button) const;

		/**
		 * @brief Set the handler for a button release event.
		 * @param button The mouse button to set the handler for.
		 * @param handler The function to call when the specified button is
		 * released.
		 * @return Reference to this MouseButtonInteraction for method chaining.
		 */
		MouseButtonInteraction &setOnButtonReleaseHandler(
			const utility::event::MouseButtonEvent::Button &button,
			const ButtonReleaseHandler &handler);

		/**
		 * @brief Get the handlers for mouse button release events.
		 * @return A map of mouse buttons to their corresponding release
		 * handlers.
		 */
		const ButtonReleaseHandler &getOnButtonReleaseHandler(
			const utility::event::MouseButtonEvent::Button &button) const;

		/**
		 * @brief Check if a specific mouse button is currently pressed.
		 * @param button The mouse button to check.
		 * @return True if the specified button is pressed, false otherwise.
		 */
		bool isButtonPressed(
			const utility::event::MouseButtonEvent::Button &button) const;

		/**
		 * @brief Set the pressed state of a specific mouse button.
		 * @param button The mouse button to set the state for.
		 * @param pressed True if the button is now pressed, false if released.
		 * @return Reference to this MouseButtonInteraction for method
		 * chaining.₩
		 */
		MouseButtonInteraction &setButtonPressed(
			const utility::event::MouseButtonEvent::Button &button,
			bool pressed);
	};

}	 // namespace guillaume::components
