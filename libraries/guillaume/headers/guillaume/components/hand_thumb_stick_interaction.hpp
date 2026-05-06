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
	 * @brief Component handling thumb stick interactions.
	 * @see systems::HandThumbStick
	 */
	class HandThumbStickInteraction: public ecs::Component
	{
		public:
		using ThumbStickHandler = std::function<void(void)>;

		private:
		ThumbStickHandler _onThumbStickHandler;
		float _x { 0.0F };
		float _y { 0.0F };
		bool _isTouched { false };
		bool _isButtonPressed { false };

		public:
		HandThumbStickInteraction(void);
		~HandThumbStickInteraction(void) = default;

		HandThumbStickInteraction &setOnThumbStickHandler(
			const ThumbStickHandler &handler);
		ThumbStickHandler getOnThumbStickHandler(void) const;

		HandThumbStickInteraction &setX(float x);
		float getX(void) const;

		HandThumbStickInteraction &setY(float y);
		float getY(void) const;

		HandThumbStickInteraction &setAxis(float x, float y);

		HandThumbStickInteraction &setTouched(bool touched);
		bool isTouched(void) const;

		HandThumbStickInteraction &setButtonPressed(bool pressed);
		bool isButtonPressed(void) const;
	};

} 	// namespace guillaume::components