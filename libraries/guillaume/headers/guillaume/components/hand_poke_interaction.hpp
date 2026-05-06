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
	 * @brief Component handling hand poke interactions.
	 * @see systems::HandPoke
	 */
	class HandPokeInteraction: public ecs::Component
	{
		public:
		using PokeHandler =
			std::function<void(void)>;	  ///< Type for poke handlers

		private:
		PokeHandler _onPokeHandler;	   ///< Handler function to call when a poke
									   ///< event occurs
		bool _isPoked { false };	   ///< Current poke state of the hand

		public:
		/**
		 * @brief Construct a HandPokeInteraction component with default
		 * handlers and states.
		 */
		HandPokeInteraction(void);

		/**
		 * @brief Destroy the HandPokeInteraction component.
		 */
		~HandPokeInteraction(void) = default;

		/**
		 * @brief Set the handler for a poke event.
		 * @param handler The function to call when a poke event occurs.
		 * @return Reference to this HandPokeInteraction for method chaining.
		 */
		HandPokeInteraction &setOnPokeHandler(const PokeHandler &handler);

		/**
		 * @brief Get the handler for a poke event.
		 * @return The function to call when a poke event occurs.
		 */
		PokeHandler getOnPokeHandler(void) const;

		/**
		 * @brief Check if the hand is currently poked.
		 * @return True if the hand is poked, false otherwise.
		 */
		bool isPoked(void) const;

		/**
		 * @brief Set the poked state of the hand.
		 * @param isPoked The new poked state to set.
		 * @return Reference to this HandPokeInteraction for method chaining.
		 */
		HandPokeInteraction &setPoked(bool isPoked);
	};

}	 // namespace guillaume::components
