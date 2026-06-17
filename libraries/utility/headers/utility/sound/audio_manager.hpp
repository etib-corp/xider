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
#include <unordered_map>
#include <thread>
#include <mutex>
#include <deque>

#include <AL/al.h>
#include <AL/alc.h>

#include "utility/sound/audio_source.hpp"

#include "utility/math/vector.hpp"

namespace utility::sound
{
	enum class AudioCommandType {
		Play,
		Stop,
		Pause,
		SetPosition,
		SetGain,
		SetPitch,
        SetLooping,
		CreateSource,
		SetBuffer,
		DestroySource
	};

    struct AudioCommand {
        AudioCommandType type;
        uint32_t sourceID;
        union {
            math::Vector3F position;
            float gain;
            float pitch;
            bool looping;
			ALuint bufferID;
        } data;
    };

	class AudioManager {
		public:
		AudioManager();
		~AudioManager();

        void submitCommand(const AudioCommand &command);

		std::unique_ptr<AudioSource>
        createAudioSource(std::shared_ptr<AudioBuffer> buffer);

		void stop();

		private:
		void threadLoop();
        void processCommands();

        void executeCommand(const AudioCommand &command);
		ALCdevice *_device;
		ALCcontext *_context;

		std::unordered_map<uint32_t, ALuint> _sources;

        std::deque<AudioCommand> _commandQueue;
        std::mutex _commandQueueMutex;

		std::thread _audioThread;
		std::mutex _sourcesMutex;
		uint32_t _nextSourceID = 1;
		bool _running = true;
	};
}	 // namespace utility::sound