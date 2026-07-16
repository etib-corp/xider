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

#include "xider/scenes/sound.hpp"
#include "xider/scenes/settings.hpp"

namespace xider::scenes
{

	Sound::Sound(std::shared_ptr<utility::RessourceProvider> ressourceProvider,
				 guillaume::LocalStorage &localStorage,
				 guillaume::SessionStorage &sessionStorage)
		: guillaume::Scene(ressourceProvider, localStorage, sessionStorage)
	{
		using namespace guillaume::entities;
		using namespace guillaume::components;

		getLogger().info() << "Sound scene created";

		auto &buttonBuilder = getBuilderManager().getBuilder<Button::Builder>();
		auto &buttonDirector =
			getDirectorManager().getDirector<Button::Director>();

		auto &textBuilder =
			getBuilderManager()
				.getBuilder<guillaume::entities::Text::Builder>();

		auto &textDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::Text::Director>();

		auto goToSettingsButton = buttonDirector.makeIconButton(
			buttonBuilder, nullptr, "Go to Settings", "settings",
			Glyph::Style::Outlined,
			[this]() {
				this->goToScene<Settings>();
			},
			Button::Color::Filled, Button::Shape::Round, Button::Size::Medium,
			false);

		auto soundText = textDirector.makeText(
			textBuilder, nullptr, "Sound Scene", 18,
			utility::graphic::Color32Bit(255, 255, 255, 255));

		auto settingsText = textDirector.makeText(
			textBuilder, nullptr, "Settings Scene", 18,
			utility::graphic::Color32Bit(255, 255, 255, 255));

		addRootEntity("sound_text", soundText);
		addRootEntity("settings_text", settingsText);

		_first_source = ressourceProvider->loadAudioSource(
			"sound/nastelbom-background-music-486996.mp3");
		_first_source->setGain(0.5f);
		_first_source->stop();

		_second_source = ressourceProvider->loadAudioSource(
			"sound/sigmamusicart-jazz-lounge-relaxing-background-music-537739.mp3");
		_second_source->setGain(0.5f);
		_second_source->stop();

		addRootEntity("pause_source_button",
					  buttonDirector.makeIconButton(
						  buttonBuilder, nullptr, "Pause First Sound", "pause",
						  Glyph::Style::Outlined,
						  [this]() {
							  _first_source->pause();
						  },
						  Button::Color::Filled, Button::Shape::Round,
						  Button::Size::Medium, false));

		addRootEntity("play_source_button",
					  buttonDirector.makeIconButton(
						  buttonBuilder, nullptr, "Play First Sound", "play_arrow",
						  Glyph::Style::Outlined,
						  [this]() {
							  _first_source->play();
						  },
						  Button::Color::Filled, Button::Shape::Round,
						  Button::Size::Medium, false));

		addRootEntity("pause_second_source_button",
					  buttonDirector.makeIconButton(
						  buttonBuilder, nullptr, "Pause Second Sound", "pause",
						  Glyph::Style::Outlined,
						  [this]() {
							  _second_source->pause();
						  },
						  Button::Color::Filled, Button::Shape::Round,
						  Button::Size::Medium, false));

		addRootEntity("play_second_source_button",
					  buttonDirector.makeIconButton(
						  buttonBuilder, nullptr, "Play Second Sound", "play_arrow",
						  Glyph::Style::Outlined,
						  [this]() {
							  _second_source->play();
						  },
						  Button::Color::Filled, Button::Shape::Round,
						  Button::Size::Medium, false));
	}

	Sound::~Sound(void)
	{
	}

}	 // namespace xider::scenes