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

#include "guillaume/components/mouse_interaction.hpp"

namespace guillaume::components
{

	MouseInteraction::MouseInteraction(void)
		: ecs::Component()
	{
		_onHover   = nullptr;
		_onUnhover = nullptr;
		_isHovered = false;

		for (const auto button:
			 { utility::event::MouseButtonEvent::Button::Left,
			   utility::event::MouseButtonEvent::Button::Middle,
			   utility::event::MouseButtonEvent::Button::Right,
			   utility::event::MouseButtonEvent::Button::X1,
			   utility::event::MouseButtonEvent::Button::X2 }) {
			_isClicked[button]				= false;
			_onClickHandlers[button]		= nullptr;
			_onClickReleaseHandlers[button] = nullptr;
		}
	}

	MouseInteraction &
		MouseInteraction::setOnHoverHandler(const HoverHandler &handler)
	{
		_onHover = handler;
		setHasChanged(true);
		return *this;
	}

	MouseInteraction &
		MouseInteraction::setOnUnhoverHandler(const UnhoverHandler &handler)
	{
		_onUnhover = handler;
		setHasChanged(true);
		return *this;
	}

	MouseInteraction::HoverHandler
		MouseInteraction::getOnHoverHandler(void) const
	{
		return _onHover;
	}

	MouseInteraction::UnhoverHandler
		MouseInteraction::getOnUnhoverHandler(void) const
	{
		return _onUnhover;
	}

	bool MouseInteraction::isHovered(void) const
	{
		return _isHovered;
	}

	MouseInteraction &MouseInteraction::setHovered(bool isHovered)
	{
		if (_isHovered == isHovered) {
			return *this;
		}
		_isHovered = isHovered;
		setHasChanged(true);
		return *this;
	}

	MouseInteraction &MouseInteraction::setOnClickHandler(
		const utility::event::MouseButtonEvent::Button &button,
		const ClickHandler &handler)
	{
		_onClickHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	const std::map<utility::event::MouseButtonEvent::Button,
				   MouseInteraction::ClickHandler> &
		MouseInteraction::getOnClickHandlers() const
	{
		return _onClickHandlers;
	}

	MouseInteraction &MouseInteraction::setOnClickReleaseHandler(
		const utility::event::MouseButtonEvent::Button &button,
		const ClickReleaseHandler &handler)
	{
		_onClickReleaseHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	const std::map<utility::event::MouseButtonEvent::Button,
				   MouseInteraction::ClickReleaseHandler> &
		MouseInteraction::getOnClickReleaseHandlers() const
	{
		return _onClickReleaseHandlers;
	}

	bool MouseInteraction::isClicked(
		const utility::event::MouseButtonEvent::Button &button) const
	{
		auto it = _isClicked.find(button);
		if (it == _isClicked.end()) {
			return false;
		}
		return it->second;
	}

	MouseInteraction &MouseInteraction::setClicked(
		const utility::event::MouseButtonEvent::Button &button, bool clicked)
	{
		if (_isClicked[button] == clicked) {
			return *this;
		}
		_isClicked[button] = clicked;
		setHasChanged(true);
		return *this;
	}

	const utility::math::Vector2F &
		MouseInteraction::getAccessibilityMargin() const
	{
		return _accessibilityMargin;
	}

	void MouseInteraction::setAccessibilityMargin(
		const utility::math::Vector2F &margin)
	{
		if (_accessibilityMargin == margin) {
			return;
		}
		_accessibilityMargin = margin;
		setHasChanged(true);
	}

}	 // namespace guillaume::components
