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
#include <guillaume/entities/text.hpp>
#include <guillaume/entities/model.hpp>

#include "xider/scenes/home.hpp"
#include "xider/scenes/settings.hpp"

namespace xider::scenes
{

	Home::Home(guillaume::LocalStorage &localStorage,
			   guillaume::SessionStorage &sessionStorage)
		: guillaume::Scene(localStorage, sessionStorage)
	{
		using namespace guillaume::entities;
		using namespace guillaume::components;

		getLogger().info() << "Home scene created";

		auto &buttonBuilder = getBuilderManager().getBuilder<Button::Builder>();
		auto &buttonDirector =
			getDirectorManager().getDirector<Button::Director>();

		auto &textBuilder =
			getBuilderManager()
				.getBuilder<guillaume::entities::Text::Builder>();
		auto &textDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::Text::Director>();

		auto &modelBuilder =
			getBuilderManager()
				.getBuilder<guillaume::entities::Model::Builder>();
		auto &modelDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::Model::Director>();

		auto goToSettingsButton = buttonDirector.makeIconButton(
			buttonBuilder, nullptr, "Go to Settings", "settings",
			Glyph::Style::Outlined,
			[this]() {
				this->goToScene<Settings>();
			},
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false);

		auto titleText = textDirector.makeText(textBuilder, nullptr, "Home Scene", 18,
							  utility::graphic::Color32Bit(255, 255, 255, 255));

		auto teapotModel = modelDirector.makeModel(modelBuilder, nullptr, "models/teapot.obj");
	}

	Home::~Home(void)
	{
	}

}	 // namespace xider::scenes