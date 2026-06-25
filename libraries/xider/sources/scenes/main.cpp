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

#include <guillaume/entities/button.hpp>
#include <guillaume/entities/panel.hpp>

#include "xider/scenes/main.hpp"
#include "xider/scenes/settings.hpp"

namespace xider::scenes
{

	Main::Main(guillaume::LocalStorage &localStorage,
			   guillaume::SessionStorage &sessionStorage)
		: guillaume::Scene(localStorage, sessionStorage)
	{
		getLogger().info() << "Main scene created";

		auto &panelBuilder =
			getBuilderManager()
				.getBuilder<guillaume::entities::Panel::Builder>();
		auto &panelDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::Panel::Director>();

		auto &buttonBuilder =
			getBuilderManager()
				.getBuilder<guillaume::entities::Button::Builder>();
		auto &buttonDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::Button::Director>();

		buttonDirector.makeButton(
			buttonBuilder, "Simple Button",
			[]() {
			},
			guillaume::entities::Button::Color::Filled,
			guillaume::entities::Button::Shape::Round,
			guillaume::entities::Button::Size::Medium, false);
		buttonDirector.makeIconButton(
			buttonBuilder, "Icon Button", "settings",
			[]() {
			},
			guillaume::entities::Button::Color::Filled,
			guillaume::entities::Button::Shape::Square,
			guillaume::entities::Button::Size::Small, false);

		buttonDirector.makeIconButton(
			buttonBuilder, "Code Button", "code",
			[]() {
			},
			guillaume::entities::Button::Color::Filled,
			guillaume::entities::Button::Shape::Round,
			guillaume::entities::Button::Size::Large, false);
	}

	Main::~Main(void)
	{
	}

}	 // namespace xider::scenes