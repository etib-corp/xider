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

#include <utility/event/hand_button_event.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component handling hand button interactions.
	 * @see systems::HandButton
	 */
	class HandButtonInteraction: public ecs::Component
	{
		public:
		static constexpr utility::event::HandButtonEvent::Button Buttons[] = {
			utility::event::HandButtonEvent::Button::X,
			utility::event::HandButtonEvent::Button::Y,
			utility::event::HandButtonEvent::Button::A,
			utility::event::HandButtonEvent::Button::B,
			utility::event::HandButtonEvent::Button::Menu,
			utility::event::HandButtonEvent::Button::System
		};	  ///< Array of hand buttons to facilitate iteration over button
			  ///< states and handlers

		public:
		using ButtonPressHandler =
			std::function<void(void)>;	  ///< Type for button press handlers
		using ButtonReleaseHandler =
			std::function<void(void)>;	  ///< Type for button release handlers

		private:
		std::map<utility::event::HandButtonEvent::Button, ButtonPressHandler>
			_onButtonPressHandlers {};	  ///< Map of button press handlers for
										  ///< each button
		std::map<utility::event::HandButtonEvent::Button, ButtonReleaseHandler>
			_onButtonReleaseHandlers {};	///< Map of button release handlers
											///< for each button
		std::map<utility::event::HandButtonEvent::Button, bool>
			_isButtonPressed {};	///< Map of button press states for each
									///< button

		public:
		/**
		 * @brief Construct a HandButtonInteraction component with default
		 * handlers and states.
		 */
		HandButtonInteraction(void);

		/**
		 * @brief Default destructor.
		 */
		~HandButtonInteraction(void) = default;

		/**
		 * @brief Set the handler for a button press event.
		 * @param button The button for which to set the handler.
		 * @param handler The function to call when the button is pressed.
		 * @return Reference to this HandButtonInteraction for method chaining.
		 */
		HandButtonInteraction &setOnButtonPressHandler(
			const utility::event::HandButtonEvent::Button &button,
			const ButtonPressHandler &handler);

		/**
		 * @brief Get the map of button press handlers.
		 * @return Map of button press handlers for each button.
		 */
		const ButtonPressHandler &getOnButtonPressHandler(
			const utility::event::HandButtonEvent::Button &button) const;

		/**
		 * @brief Set the handler for a button release event.
		 * @param button The button for which to set the handler.
		 * @param handler The function to call when the button is released.
		 * @return Reference to this HandButtonInteraction for method chaining.
		 */
		HandButtonInteraction &setOnButtonReleaseHandler(
			const utility::event::HandButtonEvent::Button &button,
			const ButtonReleaseHandler &handler);

		/**
		 * @brief Get the map of button release handlers.
		 * @return Map of button release handlers for each button.
		 */
		const ButtonReleaseHandler &getOnButtonReleaseHandler(
			const utility::event::HandButtonEvent::Button &button) const;

		/**
		 * @brief Check if a button is currently pressed.
		 * @param button The button to check.
		 * @return True if the button is pressed, false otherwise.
		 */
		bool isButtonPressed(
			const utility::event::HandButtonEvent::Button &button) const;

		/**
		 * @brief Set the pressed state of a button.
		 * @param button The button to update.
		 * @param pressed True if the button is now pressed, false if released.
		 * @return Reference to this HandButtonInteraction for method chaining.
		 */
		HandButtonInteraction &setButtonPressed(
			const utility::event::HandButtonEvent::Button &button,
			bool pressed);
	};

}	 // namespace guillaume::components
