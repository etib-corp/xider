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
	 * @brief Component handling hand pinch interactions.
	 * @see systems::HandPinch
	 */
	class HandPinchInteraction: public ecs::Component
	{
		public:
		using PinchHandler =
			std::function<void(void)>;	  ///< Type for pinch handlers

		private:
		PinchHandler _onPinchHandler;	 ///< Handler function to call when a
										 ///< pinch event occurs
		bool _isPinched { false };		 ///< Current pinch state of the hand

		public:
		/**
		 * @brief Construct a HandPinchInteraction component with default
		 * handlers and states.
		 */
		HandPinchInteraction(void);

		/**
		 * @brief Destroy the HandPinchInteraction component.
		 */
		~HandPinchInteraction(void) = default;

		/**
		 * @brief Set the handler for a pinch event.
		 * @param handler The function to call when a pinch event occurs.
		 * @return Reference to this HandPinchInteraction for method chaining.
		 */
		HandPinchInteraction &setOnPinchHandler(const PinchHandler &handler);

		/**
		 * @brief Get the current pinch event handler.
		 * @return The function set as the pinch event handler.
		 */
		PinchHandler getOnPinchHandler(void) const;

		/**
		 * @brief Check if the hand is currently pinched.
		 * @return True if the hand is pinched, false otherwise.
		 */
		bool isPinched(void) const;

		/**
		 * @brief Set the pinch state of the hand.
		 * @param isPinched The new pinch state to set.
		 * @return Reference to this HandPinchInteraction for method chaining.
		 */
		HandPinchInteraction &setPinched(bool isPinched);
	};

}	 // namespace guillaume::components
