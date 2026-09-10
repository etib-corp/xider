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

#include <string>

#include <guillaume/local_storage.hpp>

#include <utility/logging/loggable.hpp>

namespace xider
{
	/**
	 * @class Preferences
	 * @brief The settings of the demo, kept in the local storage.
	 *
	 * Every scene reads the preferences it cares about when it is entered, so
	 * that a change made in the settings scene is visible in the other scenes
	 * as soon as they are visited again, without any scene having to know
	 * about the others. The values live in the local storage, which the
	 * application opens on the machine of the user and keeps between runs.
	 *
	 * Reading a missing preference yields its default, and a storage failure
	 * is reported and treated the same way: a preference is a convenience,
	 * and the demo stays usable without it.
	 */
	class Preferences: public utility::logging::Loggable<Preferences,
														 utility::logging::
															 DefaultLogger>
	{
		public:
		/**
		 * @brief Construct the preferences of the demo.
		 * @param storage Storage holding the preferences between runs.
		 */
		explicit Preferences(guillaume::LocalStorage &storage);

		/**
		 * @brief Destroy the preferences.
		 */
		~Preferences(void);

		public:
		/**
		 * @brief Check whether the scenes show their hint lines.
		 * @return True when the hints are shown.
		 */
		bool areHintsShown(void) const noexcept;

		/**
		 * @brief Choose whether the scenes show their hint lines.
		 * @param shown True to show the hints.
		 */
		void setHintsShown(bool shown) noexcept;

		/**
		 * @brief Check whether the viewer starts with textures applied.
		 * @return True when the textures are applied.
		 */
		bool areTexturesShown(void) const noexcept;

		/**
		 * @brief Choose whether the viewer starts with textures applied.
		 * @param shown True to apply the textures.
		 */
		void setTexturesShown(bool shown) noexcept;

		/**
		 * @brief Check whether the sound scene plays at full gain.
		 * @return True when the tracks play louder.
		 */
		bool isPlaybackLoud(void) const noexcept;

		/**
		 * @brief Choose whether the sound scene plays at full gain.
		 * @param loud True to play louder.
		 */
		void setPlaybackLoud(bool loud) noexcept;

		/**
		 * @brief Forget every preference, and fall back to the defaults.
		 */
		void reset(void) noexcept;

		private:
		/**
		 * @brief Read a flag, or fall back to its default.
		 * @param key Storage key of the flag.
		 * @param fallback Value used when the flag is absent or unreadable.
		 * @return Value of the flag.
		 */
		bool readFlag(const std::string &key, bool fallback) const noexcept;

		/**
		 * @brief Write a flag.
		 * @param key Storage key of the flag.
		 * @param value Value to store.
		 */
		void writeFlag(const std::string &key, bool value) noexcept;

		private:
		/// Storage key of the hint line preference.
		static constexpr const char *_hintsKey = "xider.preferences.hints";
		/// Storage key of the texture preference.
		static constexpr const char *_texturesKey = "xider.preferences.textures";
		/// Storage key of the playback gain preference.
		static constexpr const char *_loudKey = "xider.preferences.loud";
		/// Whether the scenes show their hint lines by default.
		static constexpr bool _hintsShown = true;
		/// Whether the viewer applies textures by default.
		static constexpr bool _texturesShown = true;
		/// Whether the sound scene plays at full gain by default.
		static constexpr bool _playbackLoud = false;

		private:
		guillaume::LocalStorage
			&_storage;	///< Storage holding the preferences between runs.
	};

}	 // namespace xider
