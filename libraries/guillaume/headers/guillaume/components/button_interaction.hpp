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

#include <array>
#include <functional>
#include <map>

#include <utility/event/event.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Concept to check if a type is an enum.
	 * @tparam T The type to check.
	 * @return True if T is an enum type, false otherwise.
	 */
	template<typename T>
	concept IsEnum = std::is_enum_v<T>;

	/**
	 * @brief Generic component handling button interactions for any input type.
	 *
	 * @tparam EventType The event type (must satisfy
	 * utility::event::InheritFromEvent).
	 * @tparam ButtonEnum The enum type representing buttons in the event.
	 * @tparam N Number of buttons in the compile-time array.
	 * @tparam ButtonsArray A compile-time array of valid button values.
	 */

	template<utility::event::InheritFromEvent EventType, IsEnum ButtonEnum,
			 std::size_t N, const std::array<ButtonEnum, N> &ButtonsArray>
	class ButtonInteraction: public ecs::Component
	{
		public:
		using ButtonPressHandler   = std::function<void(void)>;
		using ButtonReleaseHandler = std::function<void(void)>;

		private:
		std::map<ButtonEnum, ButtonPressHandler> _onButtonPressHandlers {};
		std::map<ButtonEnum, ButtonReleaseHandler> _onButtonReleaseHandlers {};
		std::map<ButtonEnum, bool> _isButtonPressed {};

		public:
		ButtonInteraction(void);
		~ButtonInteraction(void) = default;

		ButtonInteraction &
			setOnButtonPressHandler(const ButtonEnum &button,
									const ButtonPressHandler &handler);
		const ButtonPressHandler &
			getOnButtonPressHandler(const ButtonEnum &button) const;

		ButtonInteraction &
			setOnButtonReleaseHandler(const ButtonEnum &button,
									  const ButtonReleaseHandler &handler);
		const ButtonReleaseHandler &
			getOnButtonReleaseHandler(const ButtonEnum &button) const;

		bool isButtonPressed(const ButtonEnum &button) const;
		ButtonInteraction &setButtonPressed(const ButtonEnum &button,
											bool pressed);
	};

}	 // namespace guillaume::components

// Include the implementation of the ButtonInteraction template class
#include "guillaume/components/button_interaction.tpp"
