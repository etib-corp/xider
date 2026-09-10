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
#include <vector>

#include "xider/scenes/home.hpp"
#include "xider/scenes/sound.hpp"

namespace xider::scenes
{
	namespace
	{
		/// Path of the first track the application ships.
		constexpr const char *ambientPath =
			"sound/nastelbom-background-music-486996.mp3";
		/// Path of the second track the application ships.
		constexpr const char *pulsePath =
			"sound/"
			"sigmamusicart-jazz-lounge-relaxing-background-music-537739.mp3";
	}	 // namespace

	Sound::Sound(std::shared_ptr<utility::RessourceProvider> ressourceProvider,
				 guillaume::LocalStorage &localStorage,
				 guillaume::SessionStorage &sessionStorage)
		: DemoScene(ressourceProvider, localStorage, sessionStorage, "Sound",
					"Two tracks played by the audio manager of the toolkit")
		, _preferences(localStorage)
	{
		getLogger().info() << "Sound scene created";

		_ambient = ressourceProvider->loadAudioSource(ambientPath);
		_pulse	 = ressourceProvider->loadAudioSource(pulsePath);

		addTrack("Ambient", _ambient.get());
		addTrack("Pulse", _pulse.get());

		addButton("go_to_home", "Home", "home", [this]() {
			this->goToScene<Home>();
		});
	}

	Sound::~Sound(void)
	{
	}

	void Sound::onEnter(void)
	{
		DemoScene::onEnter();

		applyGain();
	}

	void Sound::onExit(void)
	{
		// A track left playing would follow the user into the other scenes.
		for (utility::sound::AudioSource *track: { _ambient.get(),
													_pulse.get() }) {
			if (track != nullptr) {
				track->stop();
			}
		}

		DemoScene::onExit();
	}

	void Sound::addTrack(const std::string &title,
						 utility::sound::AudioSource *track)
	{
		addText("track_label", title, 16.0f, color::muted());

		if (track == nullptr) {
			getLogger().warning()
				<< "The track '" << title
				<< "' could not be loaded, its transport is left out";

			skipRow(0.06f);
			return;
		}

		track->setGain(_quietGain);
		track->stop();

		// The transport drives the track through its raw pointer: the entity
		// holding the handler dies with the scene, which owns the track until
		// the end of it.
		addIconRow({
			{ .name	   = "play",
			  .icon	   = "play_arrow",
			  .onClick = [track]() {
				  track->play();
			  } },
			{ .name	   = "pause",
			  .icon	   = "pause",
			  .onClick = [track]() {
				  track->pause();
			  } },
			{ .name	   = "stop",
			  .icon	   = "stop",
			  .onClick = [track]() {
				  track->stop();
			  } },
		});
	}

	void Sound::applyGain(void)
	{
		const float gain = _preferences.isPlaybackLoud() ? _loudGain
														 : _quietGain;

		for (utility::sound::AudioSource *track: { _ambient.get(),
													_pulse.get() }) {
			if (track != nullptr) {
				track->setGain(gain);
			}
		}
	}

}	 // namespace xider::scenes
