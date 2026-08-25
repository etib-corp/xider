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

#include <guillaume/scene.hpp>

namespace xider::scenes
{

	/**
	 * @brief Sound application scene.
	 *
	 * Represents the scene responsible for managing audio settings and
	 * sound-related features within the application. It provides UI and logic
	 * for adjusting volume, audio output, and other sound configurations
	 */
	class Sound: public guillaume::Scene
	{
		private:
		std::unique_ptr<utility::sound::AudioSource>
			_first_source;	  ///< Audio source for background music or sound
							  ///< effects
		std::unique_ptr<utility::sound::AudioSource>
			_second_source;	   ///< Audio source for additional sound effects or
							   ///< music
		public:
		/**
		 * @brief Construct a new Sound scene
		 * @param ressourceProvider Shared pointer to the resource provider for
		 * loading assets
		 * @param localStorage Reference to persistent local storage
		 * @param sessionStorage Reference to per-session storage
		 */
		Sound(std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			  guillaume::LocalStorage &localStorage,
			  guillaume::SessionStorage &sessionStorage);

		/**
		 * @brief Destroy the Sound scene
		 */
		~Sound(void);
	};

}	 // namespace xider::scenes