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
	 * @brief Component handling hand squeeze interactions.
	 * @see systems::HandSqueeze
	 */
	class HandSqueezeInteraction: public ecs::Component
	{
		public:
		using SqueezeHandler = std::function<void(void)>;

		private:
		SqueezeHandler _onSqueezeHandler;
		float _value { 0.0F };
		float _force { 0.0F };
		bool _isTouched { false };
		bool _isButtonPressed { false };
		bool _isSqueezed { false };

		public:
		HandSqueezeInteraction(void);
		~HandSqueezeInteraction(void) = default;

		HandSqueezeInteraction &setOnSqueezeHandler(
			const SqueezeHandler &handler);
		SqueezeHandler getOnSqueezeHandler(void) const;

		HandSqueezeInteraction &setValue(float value);
		float getValue(void) const;

		HandSqueezeInteraction &setForce(float force);
		float getForce(void) const;

		HandSqueezeInteraction &setTouched(bool touched);
		bool isTouched(void) const;

		HandSqueezeInteraction &setButtonPressed(bool pressed);
		bool isButtonPressed(void) const;

		HandSqueezeInteraction &setSqueezed(bool squeezed);
		bool isSqueezed(void) const;
	};

} 	// namespace guillaume::components