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

#include "guillaume/components/hand_squeeze_interaction.hpp"

namespace guillaume::components
{
	HandSqueezeInteraction::HandSqueezeInteraction(void)
		: ecs::Component()
	{
		_onSqueezeHandler = nullptr;
	}

	HandSqueezeInteraction &HandSqueezeInteraction::setOnSqueezeHandler(
		const SqueezeHandler &handler)
	{
		_onSqueezeHandler = handler;
		setHasChanged(true);
		return *this;
	}

	HandSqueezeInteraction::SqueezeHandler
		HandSqueezeInteraction::getOnSqueezeHandler(void) const
	{
		return _onSqueezeHandler;
	}

	HandSqueezeInteraction &HandSqueezeInteraction::setValue(float value)
	{
		if (_value == value) {
			return *this;
		}
		_value = value;
		setHasChanged(true);
		return *this;
	}

	float HandSqueezeInteraction::getValue(void) const
	{
		return _value;
	}

	HandSqueezeInteraction &HandSqueezeInteraction::setForce(float force)
	{
		if (_force == force) {
			return *this;
		}
		_force = force;
		setHasChanged(true);
		return *this;
	}

	float HandSqueezeInteraction::getForce(void) const
	{
		return _force;
	}

	HandSqueezeInteraction &HandSqueezeInteraction::setTouched(bool touched)
	{
		if (_isTouched == touched) {
			return *this;
		}
		_isTouched = touched;
		setHasChanged(true);
		return *this;
	}

	bool HandSqueezeInteraction::isTouched(void) const
	{
		return _isTouched;
	}

	HandSqueezeInteraction &HandSqueezeInteraction::setButtonPressed(bool pressed)
	{
		if (_isButtonPressed == pressed) {
			return *this;
		}
		_isButtonPressed = pressed;
		setHasChanged(true);
		return *this;
	}

	bool HandSqueezeInteraction::isButtonPressed(void) const
	{
		return _isButtonPressed;
	}

	HandSqueezeInteraction &HandSqueezeInteraction::setSqueezed(bool squeezed)
	{
		if (_isSqueezed == squeezed) {
			return *this;
		}
		_isSqueezed = squeezed;
		setHasChanged(true);
		return *this;
	}

	bool HandSqueezeInteraction::isSqueezed(void) const
	{
		return _isSqueezed;
	}

} 	// namespace guillaume::components