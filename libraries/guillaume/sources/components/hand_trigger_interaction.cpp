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

#include "guillaume/components/hand_trigger_interaction.hpp"

namespace guillaume::components
{
	HandTriggerInteraction::HandTriggerInteraction(void)
		: ecs::Component()
	{
		_onTriggerHandler = nullptr;
	}

	HandTriggerInteraction &HandTriggerInteraction::setOnTriggerHandler(
		const TriggerHandler &handler)
	{
		_onTriggerHandler = handler;
		setHasChanged(true);
		return *this;
	}

	HandTriggerInteraction::TriggerHandler
		HandTriggerInteraction::getOnTriggerHandler(void) const
	{
		return _onTriggerHandler;
	}

	HandTriggerInteraction &HandTriggerInteraction::setValue(float value)
	{
		if (_value == value) {
			return *this;
		}
		_value = value;
		setHasChanged(true);
		return *this;
	}

	float HandTriggerInteraction::getValue(void) const
	{
		return _value;
	}

	HandTriggerInteraction &HandTriggerInteraction::setTouched(bool touched)
	{
		if (_isTouched == touched) {
			return *this;
		}
		_isTouched = touched;
		setHasChanged(true);
		return *this;
	}

	bool HandTriggerInteraction::isTouched(void) const
	{
		return _isTouched;
	}

	HandTriggerInteraction &HandTriggerInteraction::setProximity(bool proximity)
	{
		if (_isProximity == proximity) {
			return *this;
		}
		_isProximity = proximity;
		setHasChanged(true);
		return *this;
	}

	bool HandTriggerInteraction::isProximity(void) const
	{
		return _isProximity;
	}

} 	// namespace guillaume::components