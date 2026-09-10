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

#include <guillaume/entities/text.hpp>

#include <utility/sound/audio_source.hpp>

#include "xider/preferences.hpp"
#include "xider/scenes/demo_scene.hpp"

namespace xider::scenes
{
	/**
	 * @brief Sound application scene.
	 *
	 * Plays the tracks the application ships, through a transport command per
	 * track, and follows the gain the preferences ask for.
	 */
	class Sound: public DemoScene
	{
		public:
		/**
		 * @brief Construct a new Sound scene.
		 * @param ressourceProvider Shared pointer to the resource provider for
		 * loading assets.
		 * @param localStorage Reference to persistent local storage.
		 * @param sessionStorage Reference to per-session storage.
		 */
		Sound(std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			  guillaume::LocalStorage &localStorage,
			  guillaume::SessionStorage &sessionStorage);

		/**
		 * @brief Destroy the Sound scene.
		 */
		~Sound(void) override;

		public:
		/**
		 * @brief Apply the preferences and stop anything left playing.
		 */
		void onEnter(void) override;

		/**
		 * @brief Stop the tracks the scene leaves behind.
		 */
		void onExit(void) override;

		private:
		/**
		 * @brief Add a track label and its transport row to the console.
		 * @param title Name of the track, shown above its transport.
		 * @param track Track the transport drives, or nullptr when the asset
		 * of the track could not be loaded.
		 */
		void addTrack(const std::string &title,
					  utility::sound::AudioSource *track);

		/**
		 * @brief Apply the gain preference to every track.
		 */
		void applyGain(void);

		private:
		static constexpr float _loudGain =
			1.0f;	///< Gain of a track when the preferences ask for it.
		static constexpr float _quietGain =
			0.5f;	///< Gain of a track otherwise.

		private:
		Preferences _preferences;	///< Settings of the demo.
		std::unique_ptr<utility::sound::AudioSource>
			_ambient {};	///< First track of the application.
		std::unique_ptr<utility::sound::AudioSource>
			_pulse {};	///< Second track of the application.
	};

}	 // namespace xider::scenes
