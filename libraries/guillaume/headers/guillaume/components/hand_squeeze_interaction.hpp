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
#include "guillaume/components/interaction.hpp"
#include "utility/event/hand_squeeze_event.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component handling hand squeeze interactions.
	 * @see systems::HandSqueeze
	 */
	class HandSqueezeInteraction:
		public Interaction<utility::event::HandSqueezeEvent>
	{
		public:
		/**
		 * @brief Constructs a new HandSqueezeInteraction instance.
		 */
		HandSqueezeInteraction(void)
			: Interaction()
		{
		}

		/**
		 * @brief Destructor for the HandSqueezeInteraction class.
		 */
		~HandSqueezeInteraction(void) = default;
	};

#if 0
	/**
	 * @brief Component handling hand squeeze interactions.
	 * @see systems::HandSqueeze
	 */
	class HandSqueezeInteraction: public ecs::Component
	{
		public:
		using SqueezeHandler = std::function<void(
			void)>;	   ///< Type alias for the squeeze event handler function.

		private:
		SqueezeHandler
			_onSqueezeHandler;	  ///< Handler function to be called when a
								  ///< squeeze interaction occurs.
		float _value {
			0.0F
		};	  ///< Current value of the squeeze interaction, typically
			  ///< representing the degree of squeeze.
		float _force {
			0.0F
		};	  ///< Current force applied during the squeeze interaction, which
			  ///< may be used to determine the intensity of the squeeze.
		bool _isTouched {
			false
		};	  ///< Indicates whether the hand is currently touching the squeeze
			  ///< interaction area, which can be used to trigger
			  ///< touch-related events or visual feedback.
		bool _isButtonPressed {
			false
		};	  ///< Indicates whether the squeeze button is currently
			  ///< pressed, which can be used to trigger button press events
			  ///< or actions based on the squeeze interaction.
		bool _isSqueezed {
			false
		};	  ///< Indicates whether the hand is currently in a squeezed state,
			  ///< which can be used to trigger squeeze-related events
			  ///< or actions when the squeeze interaction is active.

		public:
		/**
		 * @brief Constructs a new HandSqueezeInteraction instance.
		 */
		HandSqueezeInteraction(void);

		/**
		 * @brief Destructor for the HandSqueezeInteraction class.
		 */
		~HandSqueezeInteraction(void) = default;

		/**
		 * @brief Sets the handler function to be called when a squeeze
		 * interaction occurs.
		 *
		 * @param handler The function to be called when a squeeze interaction
		 * occurs. This function should take no parameters and return void.
		 * It will be invoked whenever the squeeze interaction is triggered,
		 * allowing the application to respond to the event accordingly.
		 *
		 * @return A reference to the HandSqueezeInteraction instance, allowing
		 * for method chaining.
		 */
		HandSqueezeInteraction &
			setOnSqueezeHandler(const SqueezeHandler &handler);

		/**
		 * @brief Retrieves the current handler function for squeeze
		 * interactions.
		 * @return The current squeeze handler function, which will be called
		 * when a squeeze interaction occurs.
		 */
		SqueezeHandler getOnSqueezeHandler(void) const;

		/**
		 * @brief Sets the current value of the squeeze interaction.
		 *
		 * @param value The new value for the squeeze interaction, typically
		 * representing the degree of squeeze. This value can be used to
		 * determine how much the hand is squeezed and can trigger different
		 * responses based on the intensity of the squeeze.
		 *
		 * @return A reference to the HandSqueezeInteraction instance, allowing
		 * for method chaining.
		 */
		HandSqueezeInteraction &setValue(float value);

		/**
		 * @brief Retrieves the current value of the squeeze interaction.
		 * @return The current value of the squeeze interaction, which typically
		 * represents the degree of squeeze and can be used to determine how
		 * much the hand is squeezed.
		 */
		float getValue(void) const;

		/**
		 * @brief Sets the current force applied during the squeeze interaction.
		 *
		 * @param force The new force value for the squeeze interaction, which
		 * may be used to determine the intensity of the squeeze. This value can
		 * trigger different responses based on how much force is applied during
		 * the squeeze interaction.
		 *
		 * @return A reference to the HandSqueezeInteraction instance, allowing
		 * for method chaining.
		 */
		HandSqueezeInteraction &setForce(float force);

		/**
		 * @brief Retrieves the current force applied during the squeeze
		 * interaction.
		 * @return The current force value for the squeeze interaction, which
		 * may be used to determine the intensity of the squeeze and can trigger
		 * different responses based on how much force is applied during the
		 * interaction.
		 */
		float getForce(void) const;

		/**
		 * @brief Sets whether the hand is currently touching the squeeze
		 * interaction area.
		 *
		 * @param touched A boolean value indicating whether the hand is
		 * currently touching the squeeze interaction area. This can be used to
		 * trigger touch-related events or visual feedback when the hand is in
		 * contact with the interaction area.
		 *
		 * @return A reference to the HandSqueezeInteraction instance, allowing
		 * for method chaining.
		 */
		HandSqueezeInteraction &setTouched(bool touched);

		/**
		 * @brief Retrieves whether the hand is currently touching the squeeze
		 * interaction area.
		 * @return A boolean value indicating whether the hand is currently
		 * touching the squeeze interaction area, which can be used to trigger
		 * touch-related events or visual feedback when the hand is in contact
		 * with the interaction area.
		 */
		bool isTouched(void) const;

		/**
		 * @brief Sets whether the squeeze button is currently pressed.
		 *
		 * @param pressed A boolean value indicating whether the squeeze button
		 * is currently pressed. This can be used to trigger button press events
		 * or actions based on the squeeze interaction.
		 *
		 * @return A reference to the HandSqueezeInteraction instance, allowing
		 * for method chaining.
		 */
		HandSqueezeInteraction &setButtonPressed(bool pressed);

		/**
		 * @brief Retrieves whether the squeeze button is currently pressed.
		 * @return A boolean value indicating whether the squeeze button is
		 * currently pressed, which can be used to trigger button press events
		 * or actions based on the squeeze interaction.
		 */
		bool isButtonPressed(void) const;

		/**
		 * @brief Sets whether the hand is currently in a squeezed state.
		 *
		 * @param squeezed A boolean value indicating whether the hand is
		 * currently in a squeezed state. This can be used to trigger
		 * squeeze-related events or actions when the squeeze interaction is
		 * active.
		 *
		 * @return A reference to the HandSqueezeInteraction instance, allowing
		 * for method chaining.
		 */
		HandSqueezeInteraction &setSqueezed(bool squeezed);

		/**
		 * @brief Retrieves whether the hand is currently in a squeezed state.
		 * @return A boolean value indicating whether the hand is currently in a
		 * squeezed state, which can be used to trigger squeeze-related events
		 * or actions when the squeeze interaction is active.
		 */
		bool isSqueezed(void) const;
	};
#endif

}	 // namespace guillaume::components