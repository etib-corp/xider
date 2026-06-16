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

#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>
#include <vector>

#include "utility/system_io/file.hpp"

namespace utility::sound
{

	class AudioBuffer;

	struct DecodedAudio {
		std::vector<std::byte> samples;

		uint32_t sampleRate = 0;
		uint16_t channels = 0;
		uint16_t bitsPerSample = 0;

		ALenum format = 0;
	};
	enum class AudioDecoderType { WAV, MP3, UNKNOWN };
	class AAudioDecoder {
		public:

		virtual ~AAudioDecoder() = default;

		virtual bool canDecode(const std::string &filePath) const = 0;

		virtual bool canDecode(
			AudioDecoderType type = AudioDecoderType::UNKNOWN) const = 0;

		virtual std::shared_ptr<AudioBuffer> decode(std::shared_ptr<utility::File> file) = 0;

        ALenum getALFormat(uint16_t channels, uint16_t bitsPerSample) const;
	};
}	 // namespace utility::sound