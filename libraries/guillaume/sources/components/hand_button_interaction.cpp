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

#include "guillaume/components/hand_button_interaction.hpp"

namespace guillaume::components
{

	HandButtonInteraction::HandButtonInteraction(void)
		: ecs::Component()
	{
		for (const auto button: HandButtonInteraction::Buttons) {
			_isButtonPressed[button]		 = false;
			_onButtonPressHandlers[button]	 = nullptr;
			_onButtonReleaseHandlers[button] = nullptr;
		}
	}

	HandButtonInteraction &HandButtonInteraction::setOnButtonPressHandler(
		const utility::event::HandButtonEvent::Button &button,
		const ButtonPressHandler &handler)
	{
		_onButtonPressHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	const HandButtonInteraction::ButtonPressHandler &HandButtonInteraction::getOnButtonPressHandler(
		const utility::event::HandButtonEvent::Button &button) const
	{
		auto it = _onButtonPressHandlers.find(button);
		if (it == _onButtonPressHandlers.end()) {
			throw std::invalid_argument("Invalid button");
		}
		return it->second;
	}

	HandButtonInteraction &HandButtonInteraction::setOnButtonReleaseHandler(
		const utility::event::HandButtonEvent::Button &button,
		const ButtonReleaseHandler &handler)
	{
		_onButtonReleaseHandlers[button] = handler;
		setHasChanged(true);
		return *this;
	}

	const HandButtonInteraction::ButtonReleaseHandler &HandButtonInteraction::getOnButtonReleaseHandler(
		const utility::event::HandButtonEvent::Button &button) const
	{
		auto it = _onButtonReleaseHandlers.find(button);
		if (it == _onButtonReleaseHandlers.end()) {
			throw std::invalid_argument("Invalid button");
		}
		return it->second;
	}

	bool HandButtonInteraction::isButtonPressed(
		const utility::event::HandButtonEvent::Button &button) const
	{
		auto it = _isButtonPressed.find(button);
		if (it == _isButtonPressed.end()) {
			return false;
		}
		return it->second;
	}

	HandButtonInteraction &HandButtonInteraction::setButtonPressed(
		const utility::event::HandButtonEvent::Button &button, bool pressed)
	{
		if (_isButtonPressed[button] == pressed) {
			return *this;
		}
		_isButtonPressed[button] = pressed;
		setHasChanged(true);
		return *this;
	}

}	 // namespace guillaume::components
