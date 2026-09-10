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

#include <memory>

#include <guillaume/entities/button.hpp>
#include <guillaume/entities/text.hpp>

#include "xider/preferences.hpp"
#include "xider/scenes/demo_scene.hpp"

namespace xider::scenes
{
	/**
	 * @brief Settings scene for application configuration.
	 *
	 * Toggles the preferences of the demo and writes them to the local
	 * storage, where the scenes that act on them read them back the next time
	 * they are entered.
	 */
	class Settings: public DemoScene
	{
		public:
		/**
		 * @brief Construct a new Settings scene.
		 * @param ressourceProvider Shared pointer to the resource provider.
		 * @param localStorage Reference to persistent local storage.
		 * @param sessionStorage Reference to per-session storage.
		 */
		Settings(std::shared_ptr<utility::RessourceProvider> ressourceProvider,
				 guillaume::LocalStorage &localStorage,
				 guillaume::SessionStorage &sessionStorage);

		/**
		 * @brief Destroy the Settings scene.
		 */
		~Settings(void) override;

		public:
		/**
		 * @brief Read the stored preferences back into the toggles.
		 */
		void onEnter(void) override;

		private:
		/**
		 * @brief Add a preference to the console, at the next row.
		 * @param name Name of the toggle entity.
		 * @param label Label of the toggle.
		 * @param icon Name of the icon glyph shown before the label.
		 * @param isOn Whether the preference is on.
		 * @param apply Handler storing the new value of the preference.
		 * @return The created toggle.
		 */
		std::shared_ptr<guillaume::entities::Button> addToggle(
			const std::string &name, const std::string &label,
			const std::string &icon, bool isOn,
			std::function<void(bool)> apply);

		/**
		 * @brief Show the stored preferences on the toggles.
		 */
		void applyPreferences(void);

		private:
		Preferences _preferences;	///< Settings of the demo.
		std::shared_ptr<guillaume::entities::Button>
			_hintsToggle {};	///< Toggle of the hint lines.
		std::shared_ptr<guillaume::entities::Button>
			_texturesToggle {};	   ///< Toggle of the model textures.
		std::shared_ptr<guillaume::entities::Button>
			_loudToggle {};	   ///< Toggle of the playback gain.
	};

}	 // namespace xider::scenes
