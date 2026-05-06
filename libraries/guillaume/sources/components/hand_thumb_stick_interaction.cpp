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

#include "guillaume/components/hand_thumb_stick_interaction.hpp"

namespace guillaume::components
{
	HandThumbStickInteraction::HandThumbStickInteraction(void)
		: ecs::Component()
	{
		_onThumbStickHandler = nullptr;
	}

	HandThumbStickInteraction &HandThumbStickInteraction::setOnThumbStickHandler(
		const ThumbStickHandler &handler)
	{
		_onThumbStickHandler = handler;
		setHasChanged(true);
		return *this;
	}

	HandThumbStickInteraction::ThumbStickHandler
		HandThumbStickInteraction::getOnThumbStickHandler(void) const
	{
		return _onThumbStickHandler;
	}

	HandThumbStickInteraction &HandThumbStickInteraction::setX(float x)
	{
		if (_x == x) {
			return *this;
		}
		_x = x;
		setHasChanged(true);
		return *this;
	}

	float HandThumbStickInteraction::getX(void) const
	{
		return _x;
	}

	HandThumbStickInteraction &HandThumbStickInteraction::setY(float y)
	{
		if (_y == y) {
			return *this;
		}
		_y = y;
		setHasChanged(true);
		return *this;
	}

	float HandThumbStickInteraction::getY(void) const
	{
		return _y;
	}

	HandThumbStickInteraction &HandThumbStickInteraction::setAxis(float x, float y)
	{
		setX(x);
		setY(y);
		return *this;
	}

	HandThumbStickInteraction &
		HandThumbStickInteraction::setTouched(bool touched)
	{
		if (_isTouched == touched) {
			return *this;
		}
		_isTouched = touched;
		setHasChanged(true);
		return *this;
	}

	bool HandThumbStickInteraction::isTouched(void) const
	{
		return _isTouched;
	}

	HandThumbStickInteraction &
		HandThumbStickInteraction::setButtonPressed(bool pressed)
	{
		if (_isButtonPressed == pressed) {
			return *this;
		}
		_isButtonPressed = pressed;
		setHasChanged(true);
		return *this;
	}

	bool HandThumbStickInteraction::isButtonPressed(void) const
	{
		return _isButtonPressed;
	}

} 	// namespace guillaume::components