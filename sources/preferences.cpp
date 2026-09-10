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

#include <exception>
#include <optional>
#include <string>

#include "xider/preferences.hpp"

namespace xider
{
	Preferences::Preferences(guillaume::LocalStorage &storage)
		: _storage(storage)
	{
	}

	Preferences::~Preferences(void)
	{
	}

	bool Preferences::areHintsShown(void) const noexcept
	{
		return readFlag(_hintsKey, _hintsShown);
	}

	void Preferences::setHintsShown(bool shown) noexcept
	{
		writeFlag(_hintsKey, shown);
	}

	bool Preferences::areTexturesShown(void) const noexcept
	{
		return readFlag(_texturesKey, _texturesShown);
	}

	void Preferences::setTexturesShown(bool shown) noexcept
	{
		writeFlag(_texturesKey, shown);
	}

	bool Preferences::isPlaybackLoud(void) const noexcept
	{
		return readFlag(_loudKey, _playbackLoud);
	}

	void Preferences::setPlaybackLoud(bool loud) noexcept
	{
		writeFlag(_loudKey, loud);
	}

	void Preferences::reset(void) noexcept
	{
		const char *keys[] { _hintsKey, _texturesKey, _loudKey };

		for (const char *key: keys) {
			try {
				_storage.removeItem(key);
			} catch (const std::exception &error) {
				getLogger().warning()
					<< "Could not forget the preference '" << key
					<< "': " << error.what();
			}
		}
	}

	bool Preferences::readFlag(const std::string &key,
							   bool fallback) const noexcept
	{
		try {
			const std::optional<bool> value = _storage.getItemAs<bool>(key);

			if (value.has_value()) {
				return *value;
			}
		} catch (const std::exception &error) {
			getLogger().warning() << "Could not read the preference '" << key
								  << "': " << error.what();
		}

		return fallback;
	}

	void Preferences::writeFlag(const std::string &key, bool value) noexcept
	{
		try {
			_storage.setItem(key, value);
		} catch (const std::exception &error) {
			getLogger().warning() << "Could not store the preference '" << key
								  << "': " << error.what();
		}
	}

}	 // namespace xider
