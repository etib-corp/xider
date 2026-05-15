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
	 * @brief Component handling thumb rest interactions.
	 * @see systems::HandThumbRest
	 */
	class HandThumbRestInteraction: public ecs::Component
	{
		public:
		using ThumbRestHandler =
			std::function<void(void)>;	  ///< Type alias for the thumb rest
										  ///< event handler function.

		private:
		ThumbRestHandler
			_onThumbRestHandler;	///< Handler function to be called when a
									///< thumb rest interaction occurs.
		bool _isTouched {
			false
		};	  ///< Indicates whether the hand is currently touching the thumb
			  ///< rest interaction area, which can be used to trigger
			  ///< touch-related events or visual feedback when the
			  ///< hand is in contact with the interaction area.
		bool _isProximity {
			false
		};	  ///< Indicates whether the hand is currently in proximity to the
			  ///< thumb rest interaction area, which can be used to trigger
			  ///< proximity-related events or visual feedback when the hand is
			  ///< near the interaction area.

		public:
		/**
		 * @brief Constructs a HandThumbRestInteraction component.
		 */
		HandThumbRestInteraction(void);

		/**
		 * @brief Destructor for the HandThumbRestInteraction component.
		 */
		~HandThumbRestInteraction(void) = default;

		/**
		 * @brief Sets the handler function to be called when a thumb rest
		 * interaction occurs.
		 *
		 * @param handler The function to be called when a thumb rest
		 * interaction occurs. This can be used to trigger specific actions or
		 * events in response to the thumb rest interaction.
		 *
		 * @return A reference to the HandThumbRestInteraction instance,
		 * allowing for method chaining.
		 */
		HandThumbRestInteraction &
			setOnThumbRestHandler(const ThumbRestHandler &handler);

		/**
		 * @brief Retrieves the current handler function for thumb rest
		 * interactions.
		 * @return The current thumb rest handler function, which will be called
		 * when a thumb rest interaction occurs.
		 */
		ThumbRestHandler getOnThumbRestHandler(void) const;

		/**
		 * @brief Sets whether the hand is currently touching the thumb rest
		 * interaction area.
		 *
		 * @param touched A boolean value indicating whether the hand is
		 * currently touching the thumb rest interaction area. This can be used
		 * to trigger touch-related events or visual feedback when the hand is
		 * in contact with the interaction area.
		 *
		 * @return A reference to the HandThumbRestInteraction instance,
		 * allowing for method chaining.
		 */
		HandThumbRestInteraction &setTouched(bool touched);

		/**
		 * @brief Retrieves whether the hand is currently touching the thumb
		 * rest interaction area.
		 * @return A boolean value indicating whether the hand is currently
		 * touching the thumb rest interaction area, which can be used to
		 * trigger touch-related events or visual feedback when the hand is in
		 * contact with the interaction area.
		 */
		bool isTouched(void) const;

		/**
		 * @brief Sets whether the hand is currently in proximity to the thumb
		 * rest interaction area.
		 *
		 * @param proximity A boolean value indicating whether the hand is
		 * currently in proximity to the thumb rest interaction area. This can
		 * be used to trigger proximity-related events or visual feedback when
		 * the hand is near the interaction area.
		 *
		 * @return A reference to the HandThumbRestInteraction instance,
		 * allowing for method chaining.
		 */
		HandThumbRestInteraction &setProximity(bool proximity);

		/**
		 * @brief Retrieves whether the hand is currently in proximity to the
		 * thumb rest interaction area.
		 * @return A boolean value indicating whether the hand is currently in
		 * proximity to the thumb rest interaction area, which can be used to
		 * trigger proximity-related events or visual feedback when the hand is
		 * near the interaction area.
		 */
		bool isProximity(void) const;
	};

}	 // namespace guillaume::components