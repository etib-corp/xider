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

#include <memory>

#include "utility/sound/audio_buffer.hpp"

namespace utility::sound
{
    class AudioManager;

    class AudioSource {

        public:
            AudioSource(uint32_t sourceID, AudioManager &manager);
            ~AudioSource();

            void play();

            void pause();

            void stop();

            void setBuffer(std::shared_ptr<AudioBuffer> buffer);

            void setLooping(bool loop);

            void setPitch(float pitch);

            void setGain(float gain);

            uint32_t sourceID() const;

            ALuint alId() const;

        private:
            std::shared_ptr<AudioBuffer> _buffer;

            ALuint _alId = 0;

            uint32_t _sourceID;
            AudioManager &_manager;

            bool _isPlaying = false;
            float _pitch = 1.0f;
            float _gain = 1.0f;
            bool _looping = false;
    };
}