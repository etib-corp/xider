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
	 * @brief Component handling hand trigger interactions.
	 * @see systems::HandTrigger
	 */
	class HandTriggerInteraction: public ecs::Component
	{
		public:
		using TriggerHandler = std::function<void(
			void)>;	   ///< Type alias for the trigger event handler function.

		private:
		TriggerHandler
			_onTriggerHandler;	  ///< Handler function to be called when a
								  ///< trigger interaction occurs.
		float _value {
			0.0F
		};	  ///< Current value of the trigger interaction, typically
			  ///< representing the degree of trigger pull.
		bool _isTouched {
			false
		};	  ///< Indicates whether the hand is currently touching the trigger
			  ///< interaction area, which can be used to trigger touch-related
			  ///< events or visual feedback.
		bool _isProximity {
			false
		};	  ///< Indicates whether the hand is currently in proximity to the
			  ///< trigger interaction area, which can be used to trigger
			  ///< proximity-related events or visual feedback when the hand is
			  ///< near the interaction area.

		public:
		/**
		 * @brief Constructs a HandTriggerInteraction component.
		 */
		HandTriggerInteraction(void);

		/**
		 * @brief Destructor for the HandTriggerInteraction component.
		 */
		~HandTriggerInteraction(void) = default;

		/**
		 * @brief Sets the handler function to be called when a trigger
		 * interaction occurs.
		 *
		 * @param handler The new trigger handler function, which will be called
		 * when a trigger interaction occurs. This can be used to define custom
		 * behavior in response to trigger interactions.
		 *
		 * @return A reference to the HandTriggerInteraction instance, allowing
		 * for method chaining.
		 */
		HandTriggerInteraction &
			setOnTriggerHandler(const TriggerHandler &handler);

		/**
		 * @brief Retrieves the current handler function for trigger
		 * interactions.
		 *
		 * @return The current trigger handler function, which will be called
		 * when a trigger interaction occurs.
		 */
		TriggerHandler getOnTriggerHandler(void) const;

		/**
		 * @brief Sets the current value of the trigger interaction.
		 *
		 * @param value The new value for the trigger interaction, typically
		 * representing the degree of trigger pull. This value can be used to
		 * determine how much the trigger is pulled and can trigger different
		 * responses based on the intensity of the pull.
		 *
		 * @return A reference to the HandTriggerInteraction instance, allowing
		 * for method chaining.
		 */
		HandTriggerInteraction &setValue(float value);

		/**
		 * @brief Retrieves the current value of the trigger interaction.
		 * @return The current value of the trigger interaction, which typically
		 * represents the degree of trigger pull and can be used to determine
		 * how much the trigger is pulled.
		 */
		float getValue(void) const;

		/**
		 * @brief Sets whether the hand is currently touching the trigger
		 * interaction area.
		 *
		 * @param touched A boolean value indicating whether the hand is
		 * currently touching the trigger interaction area. This can be used to
		 * trigger touch-related events or visual feedback when the hand is in
		 * contact with the interaction area.
		 *
		 * @return A reference to the HandTriggerInteraction instance, allowing
		 * for method chaining.
		 */
		HandTriggerInteraction &setTouched(bool touched);

		/**
		 * @brief Retrieves whether the hand is currently touching the trigger
		 * interaction area.
		 * @return A boolean value indicating whether the hand is currently
		 * touching the trigger interaction area, which can be used to trigger
		 * touch-related events or visual feedback when the hand is in contact
		 * with the interaction area.
		 */
		bool isTouched(void) const;

		/**
		 * @brief Sets whether the hand is currently in proximity to the trigger
		 * interaction area.
		 *
		 * @param proximity A boolean value indicating whether the hand is
		 * currently in proximity to the trigger interaction area. This can be
		 * used to trigger proximity-related events or visual feedback when the
		 * hand is near the interaction area.
		 *
		 * @return A reference to the HandTriggerInteraction instance, allowing
		 * for method chaining.
		 */
		HandTriggerInteraction &setProximity(bool proximity);

		/**
		 * @brief Retrieves whether the hand is currently in proximity to the
		 * trigger interaction area.
		 * @return A boolean value indicating whether the hand is currently in
		 * proximity to the trigger interaction area, which can be used to
		 * trigger proximity-related events or visual feedback when the hand is
		 * near the interaction area.
		 */
		bool isProximity(void) const;
	};

}	 // namespace guillaume::components