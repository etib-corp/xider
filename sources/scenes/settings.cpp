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

#include <string>

#include "xider/scenes/home.hpp"
#include "xider/scenes/settings.hpp"

namespace xider::scenes
{
	Settings::Settings(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		guillaume::LocalStorage &localStorage,
		guillaume::SessionStorage &sessionStorage)
		: DemoScene(ressourceProvider, localStorage, sessionStorage, "Settings",
					"Preferences the demo remembers from one run to the next")
		, _preferences(localStorage)
	{
		getLogger().info() << "Settings scene created";

		_hintsToggle = addToggle("toggle_hints", "Hints", "tips_and_updates",
								 _preferences.areHintsShown(), [this](bool isOn) {
									 this->_preferences.setHintsShown(isOn);
								 });

		_texturesToggle =
			addToggle("toggle_textures", "Textures", "texture",
					  _preferences.areTexturesShown(), [this](bool isOn) {
						  this->_preferences.setTexturesShown(isOn);
					  });

		_loudToggle =
			addToggle("toggle_loud", "Loud playback", "volume_up",
					  _preferences.isPlaybackLoud(), [this](bool isOn) {
						  this->_preferences.setPlaybackLoud(isOn);
					  });

		addButton("reset_preferences", "Reset", "autorenew", [this]() {
			this->_preferences.reset();
			this->applyPreferences();
		});

		addButton("go_to_home", "Home", "home", [this]() {
			this->goToScene<Home>();
		});
	}

	Settings::~Settings(void)
	{
	}

	void Settings::onEnter(void)
	{
		DemoScene::onEnter();

		applyPreferences();
	}

	std::shared_ptr<guillaume::entities::Button> Settings::addToggle(
		const std::string &name, const std::string &label, const std::string &icon,
		bool isOn, std::function<void(bool)> apply)
	{
		// The toggle shows its state with the glyph of a checked box, and reads
		// it back from the button itself: the button flips its own state before
		// it runs the handler it was given.
		auto toggle = addButton(name, label, icon, []() {},
								guillaume::entities::Button::Size::Medium, true);

		toggle->setSelectedIconGlyphName("check");
		toggle->setSelected(isOn);

		guillaume::entities::Button *state = toggle.get();

		toggle->setOnClick([state, apply = std::move(apply)]() {
			apply(state->isSelected());
		});

		return toggle;
	}

	void Settings::applyPreferences(void)
	{
		_hintsToggle->setSelected(_preferences.areHintsShown());
		_texturesToggle->setSelected(_preferences.areTexturesShown());
		_loudToggle->setSelected(_preferences.isPlaybackLoud());
	}

}	 // namespace xider::scenes
