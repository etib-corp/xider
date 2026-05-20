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

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{
	/**
	 * @brief Component handling thumb stick interactions.
	 * @see systems::HandThumbStick
	 */
	class HandThumbStickInteraction: public ecs::Component
	{
		public:
		using ThumbStickHandler =
			std::function<void(void)>;	  ///< Type alias for the thumb stick
										  ///< event handler function.

		private:
		ThumbStickHandler
			_onThumbStickHandler;	 ///< Handler function to be called when a
									 ///< thumb stick interaction occurs.
		float _x { 0.0F };	  ///< Current X-axis value of the thumb stick
							  ///< interaction, typically representing the
							  ///< horizontal position of the thumb stick.
		float _y { 0.0F };	  ///< Current Y-axis value of the thumb stick
							  ///< interaction, typically representing the
							  ///< vertical position of the thumb stick.
		bool _isTouched {
			false
		};	  ///< Indicates whether the hand is currently touching the thumb
			  ///< stick interaction area, which can be used to trigger
			  ///< touch-related events or visual feedback when the
			  ///< hand is in contact with the interaction area.
		bool _isButtonPressed {
			false
		};	  ///< Indicates whether the thumb stick button is currently
			  ///< pressed, which can be used to trigger button-related events
			  ///< or visual feedback when the thumb stick is pressed.

		public:
		/**
		 * @brief Constructs a HandThumbStickInteraction component.
		 */
		HandThumbStickInteraction(void);

		/**
		 * @brief Destructor for the HandThumbStickInteraction component.
		 */
		~HandThumbStickInteraction(void) = default;

		/**
		 * @brief Sets the handler function to be called when a thumb stick
		 * interaction occurs.
		 *
		 * @param handler The function to be called when a thumb stick
		 * interaction occurs. This can be used to trigger specific actions or
		 * responses based on thumb stick input.
		 *
		 * @return A reference to the HandThumbStickInteraction instance,
		 * allowing for method chaining.
		 */
		HandThumbStickInteraction &
			setOnThumbStickHandler(const ThumbStickHandler &handler);

		/**
		 * @brief Retrieves the current handler function for thumb stick
		 * interactions.
		 * @return The current thumb stick handler function, which will be
		 * called when a thumb stick interaction occurs.
		 */
		ThumbStickHandler getOnThumbStickHandler(void) const;

		/**
		 * @brief Sets the X-axis value for the thumb stick interaction.
		 *
		 * @param x The new X-axis value for the thumb stick interaction, which
		 * typically represents the horizontal position of the thumb stick. This
		 * value can be used to determine the direction and intensity of thumb
		 * stick input along the horizontal axis.
		 *
		 * @return A reference to the HandThumbStickInteraction instance,
		 * allowing for method chaining.
		 */
		HandThumbStickInteraction &setX(float x);

		/**
		 * @brief Retrieves the current X-axis value of the thumb stick
		 * interaction.
		 * @return The current X-axis value for the thumb stick interaction,
		 * which typically represents the horizontal position of the thumb
		 * stick and can be used to determine the direction and intensity of
		 * thumb stick input along the horizontal axis.
		 */
		float getX(void) const;

		/**
		 * @brief Sets the Y-axis value for the thumb stick interaction.
		 *
		 * @param y The new Y-axis value for the thumb stick interaction, which
		 * typically represents the vertical position of the thumb stick. This
		 * value can be used to determine the direction and intensity of thumb
		 * stick input along the vertical axis.
		 *
		 * @return A reference to the HandThumbStickInteraction instance,
		 * allowing for method chaining.
		 */
		HandThumbStickInteraction &setY(float y);

		/**
		 * @brief Retrieves the current Y-axis value of the thumb stick
		 * interaction.
		 * @return The current Y-axis value for the thumb stick interaction,
		 * which typically represents the vertical position of the thumb stick
		 * and can be used to determine the direction and intensity of thumb
		 * stick input along the vertical axis.
		 */
		float getY(void) const;

		/**
		 * @brief Sets both the X and Y-axis values for the thumb stick
		 * interaction.
		 *
		 * @param x The new X-axis value for the thumb stick interaction, which
		 * typically represents the horizontal position of the thumb stick.
		 * @param y The new Y-axis value for the thumb stick interaction, which
		 * typically represents the vertical position of the thumb stick. This
		 * value can be used to determine the direction and intensity of thumb
		 * stick input along both axes.
		 *
		 * @return A reference to the HandThumbStickInteraction instance,
		 * allowing for method chaining.
		 */
		HandThumbStickInteraction &setAxis(float x, float y);

		/**
		 * @brief Sets whether the hand is currently touching the thumb stick
		 * interaction area.
		 *
		 * @param touched A boolean value indicating whether the hand is
		 * currently touching the thumb stick interaction area. This can be used
		 * to trigger touch-related events or visual feedback when the hand is
		 * in contact with the interaction area.
		 *
		 * @return A reference to the HandThumbStickInteraction instance,
		 * allowing for method chaining.
		 */
		HandThumbStickInteraction &setTouched(bool touched);

		/**
		 * @brief Retrieves whether the hand is currently touching the thumb
		 * stick interaction area.
		 * @return A boolean value indicating whether the hand is currently
		 * touching the thumb stick interaction area, which can be used to
		 * trigger touch-related events or visual feedback when the hand is in
		 * contact with the interaction area.
		 */
		bool isTouched(void) const;

		/**
		 * @brief Sets whether the thumb stick button is currently pressed.
		 *
		 * @param pressed A boolean value indicating whether the thumb stick
		 * button is currently pressed. This can be used to trigger
		 * button-related events or visual feedback when the thumb stick is
		 * pressed.
		 *
		 * @return A reference to the HandThumbStickInteraction instance,
		 * allowing for method chaining.
		 */
		HandThumbStickInteraction &setButtonPressed(bool pressed);

		/**
		 * @brief Retrieves whether the thumb stick button is currently pressed.
		 * @return A boolean value indicating whether the thumb stick button is
		 * currently pressed, which can be used to trigger button-related events
		 * or visual feedback when the thumb stick is pressed.
		 */
		bool isButtonPressed(void) const;
	};

}	 // namespace guillaume::components