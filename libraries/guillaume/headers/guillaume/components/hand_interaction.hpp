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
#include <utility/math/vector.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component handling hand interactions (hover, click, pinch, poke).
	 * @see systems::HandInteraction
	 */
	class HandInteraction: public ecs::Component
	{
		public:
		using HoverHandler = std::function<void(void)>;		 ///< Hover event
															 ///< handler type
		using UnhoverHandler = std::function<void(void)>;	 ///< Unhover event
															 ///< handler type
		using ClickHandler =
			std::function<void(void)>;	  ///< Button click
										  ///< event handler type
		using ClickReleaseHandler =
			std::function<void(void)>;	  ///< Button release event handler
										  ///< type

		using PinchHandler =
			std::function<void(void)>;	  ///< Hand pinch
										  ///< event handler type
		using PokeHandler = std::function<void(void)>;	  ///< Hand poke event
														  ///< handler type

		private:
		HoverHandler _onHover;		  ///< Hover event handler
		UnhoverHandler _onUnhover;	  ///< Unhover event handler
		bool _isHovered { false };	  ///< Flag indicating if currently hovered

		std::map<utility::event::HandButtonEvent::Button, ClickHandler>
			_onClickHandlers {};	///< Hand button click event handlers
		std::map<utility::event::HandButtonEvent::Button, ClickReleaseHandler>
			_onClickReleaseHandlers {};	   ///< Hand button release
										   ///< event handlers
		std::map<utility::event::HandButtonEvent::Button, bool>
			_isClicked {};	  ///< Flag indicating if currently clicked

		PinchHandler _onPinchHandler;	 ///< Hand pinch event handler
		bool _isPinched { false };		 ///< Flag indicating if currently
										 ///< pinched

		PokeHandler _onPokeHandler;	   ///< Hand poke event handler
		bool _isPoked { false };	   ///< Flag indicating if currently
									   ///< poked

		utility::math::Vector2F _accessibilityMargin {
			0.0f, 0.0f
		};	  ///< Margin
			  ///< to
			  ///< increase
			  ///< the
			  ///< interactive
			  ///< area

		public:
		/**
		 * @brief Default constructor for the HandInteraction component.
		 */
		HandInteraction(void);

		/**
		 * @brief Default destructor for the HandInteraction component.
		 */
		~HandInteraction(void) = default;

		/**
		 * @brief Set the onHover event handler.
		 * @param handler The function to call on hover enter.
		 * @return Reference to this component for chaining.
		 */
		HandInteraction &setOnHoverHandler(const HoverHandler &handler);

		/**
		 * @brief Set the onUnhover event handler.
		 * @param handler The function to call on hover leave.
		 * @return Reference to this component for chaining.
		 */
		HandInteraction &setOnUnhoverHandler(const UnhoverHandler &handler);

		/**
		 * @brief Get the onHover event handler.
		 * @return The hover enter handler.
		 */
		HoverHandler getOnHoverHandler(void) const;

		/**
		 * @brief Get the onUnhover event handler.
		 * @return The hover leave handler.
		 */
		UnhoverHandler getOnUnhoverHandler(void) const;

		/**
		 * @brief Check if currently hovered.
		 * @return True when hand pointer is inside entity bounds.
		 */
		bool isHovered(void) const;

		/**
		 * @brief Set current hovered state.
		 * @param isHovered Current hovered state.
		 * @return Reference to this component for chaining.
		 */
		HandInteraction &setHovered(bool isHovered);

		/**
		 * @brief Set the onClick event handler for one hand button.
		 * @param button The hand button to associate with the handler.
		 * @param handler The function to call on click events.
		 * @return Reference to this component for chaining.
		 */
		HandInteraction &setOnClickHandler(
			const utility::event::HandButtonEvent::Button &button,
			const ClickHandler &handler);

		/**
		 * @brief Get click handlers indexed by hand button.
		 * @return A map of hand buttons to click handlers.
		 */
		const std::map<utility::event::HandButtonEvent::Button, ClickHandler> &
			getOnClickHandlers() const;

		/**
		 * @brief Set the onRelease event handler for one hand button.
		 * @param button The hand button to associate with the handler.
		 * @param handler The function to call on release events.
		 * @return Reference to this component for chaining.
		 */
		HandInteraction &setOnClickReleaseHandler(
			const utility::event::HandButtonEvent::Button &button,
			const ClickReleaseHandler &handler);

		/**
		 * @brief Get click release handlers indexed by hand button.
		 * @return A map of hand buttons to release handlers.
		 */
		const std::map<utility::event::HandButtonEvent::Button,
					   ClickReleaseHandler> &
			getOnClickReleaseHandlers() const;

		/**
		 * @brief Check if currently clicked for one hand button.
		 * @param button The hand button to check.
		 * @return True if clicked for the provided button.
		 */
		bool isClicked(
			const utility::event::HandButtonEvent::Button &button) const;

		/**
		 * @brief Set clicked state for one hand button.
		 * @param button The hand button to update.
		 * @param clicked Clicked state to set.
		 * @return Reference to this component for chaining.
		 */
		HandInteraction &
			setClicked(const utility::event::HandButtonEvent::Button &button,
					   bool clicked);

		/**
		 * @brief Set the onPinch event handler.
		 * @param handler The function to call on hand pinch events.
		 * @return Reference to this component for chaining.
		 */
		HandInteraction &setOnPinchHandler(const PinchHandler &handler);

		/**
		 * @brief Get the onPinch event handler.
		 * @return The hand pinch event handler.
		 */
		PinchHandler getOnPinchHandler(void) const;

		/**
		 * @brief Check if currently pinched.
		 * @return True if pinched.
		 */
		bool isPinched(void) const;

		/**
		 * @brief Set pinched state.
		 * @param isPinched Pinched state to set.
		 * @return Reference to this component for chaining.
		 */
		HandInteraction &setPinched(bool isPinched);

		/**
		 * @brief Set the onPoke event handler.
		 * @param handler The function to call on hand poke events.
		 * @return Reference to this component for chaining.
		 */
		HandInteraction &setOnPokeHandler(const PokeHandler &handler);

		/**
		 * @brief Get the onPoke event handler.
		 * @return The hand poke event handler.
		 */
		PokeHandler getOnPokeHandler(void) const;

		/**
		 * @brief Check if currently poked.
		 * @return True if poked.
		 */
		bool isPoked(void) const;

		/**
		 * @brief Set poked state.
		 * @param isPoked Poked state to set.
		 * @return Reference to this component for chaining.
		 */
		HandInteraction &setPoked(bool isPoked);

		/**
		 * @brief Get the Accessibility Margin.
		 * @return The margin to increase the interactive area.
		 */
		const utility::math::Vector2F &getAccessibilityMargin() const;

		/**
		 * @brief Set the Accessibility Margin.
		 * @param margin The margin to increase the interactive area.
		 */
		void setAccessibilityMargin(const utility::math::Vector2F &margin);
	};

}	 // namespace guillaume::components
