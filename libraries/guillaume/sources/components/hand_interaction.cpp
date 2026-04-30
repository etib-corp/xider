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

#include "guillaume/components/hand_interaction.hpp"

namespace guillaume::components
{

	HandInteraction::HandInteraction(void)
		: ecs::Component()
	{
		_onHover   = nullptr;
		_onUnhover = nullptr;
		_isHovered = false;

		for (const auto button:
			 { utility::event::HandButtonEvent::Button::X,
			   utility::event::HandButtonEvent::Button::Y,
			   utility::event::HandButtonEvent::Button::A,
			   utility::event::HandButtonEvent::Button::B,
			   utility::event::HandButtonEvent::Button::Menu,
			   utility::event::HandButtonEvent::Button::System }) {
			_isClicked[button]				= false;
			_onClickHandlers[button]		= nullptr;
			_onClickReleaseHandlers[button] = nullptr;
		}

		_onPinchHandler = nullptr;
		_isPinched		= false;
		_onPokeHandler	= nullptr;
		_isPoked		= false;
	}

	HandInteraction &
		HandInteraction::setOnHoverHandler(const HoverHandler &handler)
	{
		_onHover = handler;
		setHasChanged(true);
		return *this;
	}

	HandInteraction &
		HandInteraction::setOnUnhoverHandler(const UnhoverHandler &handler)
	{
		_onUnhover = handler;
		setHasChanged(true);
		return *this;
	}

	HandInteraction::HoverHandler HandInteraction::getOnHoverHandler(void) const
	{
		return _onHover;
	}

	HandInteraction::UnhoverHandler
		HandInteraction::getOnUnhoverHandler(void) const
	{
		return _onUnhover;
	}

	bool HandInteraction::isHovered(void) const
	{
		return _isHovered;
	}

	HandInteraction &HandInteraction::setHovered(bool isHovered)
	{
		if (_isHovered == isHovered) {
			return *this;
		}
		_isHovered = isHovered;
		setHasChanged(true);
		return *this;
	}

	HandInteraction &HandInteraction::setOnClickHandler(
		const utility::event::HandButtonEvent::Button &button,
		const ClickHandler &handler)
	{
		_onClickHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	const std::map<utility::event::HandButtonEvent::Button,
				   HandInteraction::ClickHandler> &
		HandInteraction::getOnClickHandlers() const
	{
		return _onClickHandlers;
	}

	HandInteraction &HandInteraction::setOnClickReleaseHandler(
		const utility::event::HandButtonEvent::Button &button,
		const ClickReleaseHandler &handler)
	{
		_onClickReleaseHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	const std::map<utility::event::HandButtonEvent::Button,
				   HandInteraction::ClickReleaseHandler> &
		HandInteraction::getOnClickReleaseHandlers() const
	{
		return _onClickReleaseHandlers;
	}

	bool HandInteraction::isClicked(
		const utility::event::HandButtonEvent::Button &button) const
	{
		auto it = _isClicked.find(button);
		if (it == _isClicked.end()) {
			return false;
		}
		return it->second;
	}

	HandInteraction &HandInteraction::setClicked(
		const utility::event::HandButtonEvent::Button &button, bool clicked)
	{
		if (_isClicked[button] == clicked) {
			return *this;
		}
		_isClicked[button] = clicked;
		setHasChanged(true);
		return *this;
	}

	HandInteraction &
		HandInteraction::setOnPinchHandler(const PinchHandler &handler)
	{
		_onPinchHandler = handler;
		setHasChanged(true);
		return *this;
	}

	HandInteraction::PinchHandler HandInteraction::getOnPinchHandler(void) const
	{
		return _onPinchHandler;
	}

	bool HandInteraction::isPinched(void) const
	{
		return _isPinched;
	}

	HandInteraction &HandInteraction::setPinched(bool isPinched)
	{
		if (_isPinched == isPinched) {
			return *this;
		}
		_isPinched = isPinched;
		setHasChanged(true);
		return *this;
	}

	HandInteraction &
		HandInteraction::setOnPokeHandler(const PokeHandler &handler)
	{
		_onPokeHandler = handler;
		setHasChanged(true);
		return *this;
	}

	HandInteraction::PokeHandler HandInteraction::getOnPokeHandler(void) const
	{
		return _onPokeHandler;
	}

	bool HandInteraction::isPoked(void) const
	{
		return _isPoked;
	}

	HandInteraction &HandInteraction::setPoked(bool isPoked)
	{
		if (_isPoked == isPoked) {
			return *this;
		}
		_isPoked = isPoked;
		setHasChanged(true);
		return *this;
	}

	const utility::math::Vector2F &
		HandInteraction::getAccessibilityMargin() const
	{
		return _accessibilityMargin;
	}

	void HandInteraction::setAccessibilityMargin(
		const utility::math::Vector2F &margin)
	{
		if (_accessibilityMargin == margin) {
			return;
		}
		_accessibilityMargin = margin;
		setHasChanged(true);
	}

}	 // namespace guillaume::components
