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
#include "utility/sound/audio_manager.hpp"

utility::sound::AudioManager::AudioManager()
{
	device_ = alcOpenDevice(nullptr);
	if (!device_) {
		throw std::runtime_error("Failed to open audio device");
	}

	context_ = alcCreateContext(device_, nullptr);
	if (!context_) {
		alcCloseDevice(device_);
		throw std::runtime_error("Failed to create audio context");
	}

	alcMakeContextCurrent(context_);

	_audioThread = std::thread(&AudioManager::threadLoop, this);
}

utility::sound::AudioManager::~AudioManager()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context_);
	alcCloseDevice(device_);

    stop();

	if (_audioThread.joinable()) {
		_audioThread.join();
	}
}

void utility::sound::AudioManager::submitCommand(const AudioCommand &command)
{
    std::lock_guard<std::mutex> lock(_commandQueueMutex);
    _commandQueue.push_back(command);
}

std::shared_ptr<utility::sound::AudioSource>
	utility::sound::AudioManager::createAudioSource(
		std::shared_ptr<AudioBuffer> buffer)
{
	auto audioSource = std::make_shared<AudioSource>(_nextSourceID++, *this);
	audioSource->setBuffer(buffer);

    std::lock_guard<std::mutex> lock(_sourcesMutex);
    _sources[audioSource->sourceID()] = audioSource;

	return audioSource;
}

void utility::sound::AudioManager::stop()
{
    _running = false;
}

void utility::sound::AudioManager::threadLoop()
{
	while (_running) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
        processCommands();
	}
}

void utility::sound::AudioManager::processCommands()
{
    std::lock_guard<std::mutex> lock(_commandQueueMutex);
    while (!_commandQueue.empty()) {
        executeCommand(_commandQueue.front());
        _commandQueue.pop_front();
    }
}

void utility::sound::AudioManager::executeCommand(const AudioCommand &command)
{
    std::lock_guard<std::mutex> lock(_sourcesMutex);
    auto it = _sources.find(command.sourceID);
    if (it == _sources.end()) {
        std::cerr << "Audio source with ID " << command.sourceID << " not found" << std::endl;
        return;
    }

    auto &source = it->second;

    switch (command.type) {
        case AudioCommandType::Play:
            alSourcePlay(source->alId());
            break;
        case AudioCommandType::Stop:
            alSourceStop(source->alId());
            break;
        case AudioCommandType::Pause:
            alSourcePause(source->alId());
            break;
        case AudioCommandType::SetPosition:
            // source->setPosition(command.data.position);
            break;
        case AudioCommandType::SetGain:
            alSourcef(source->alId(), AL_GAIN, command.data.gain);
            break;
        case AudioCommandType::SetPitch:
            alSourcef(source->alId(), AL_PITCH, command.data.pitch);
            break;
        case AudioCommandType::SetLooping:
            alSourcei(source->alId(), AL_LOOPING, command.data.looping ? AL_TRUE : AL_FALSE);
            break;
    }
}