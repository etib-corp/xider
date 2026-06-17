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

#include <utility/logging/loggable.hpp>
#include <utility/logging/default_logger.hpp>

namespace utility::sound
{
    /**
     * @enum AudioCommandType
     *
     * @brief Enumerates the commands processed by the audio thread.
     *
     * Audio commands are submitted from arbitrary threads through
     * AudioManager::submitCommand() and executed asynchronously on the
     * dedicated audio thread.
     */
    enum class AudioCommandType {
        Play,				//< Start playback of the audio source.
        Stop,				//< Stop playback of the audio source.
        Pause,				//< Pause playback of the audio source.
        SetPosition,		//< Update the world-space position of the audio source.
        SetGain,			//< Update the gain multiplier of the audio source.
        SetPitch,			//< Update the pitch multiplier of the audio source.
        SetLooping,			//< Update the looping state of the audio source.
        CreateSource, 		//< Create a new audio source with an initial buffer.
        SetBuffer,			//< Assign a new audio buffer to the audio source.
        DestroySource		//< Destroy the audio source and release its OpenAL resources.
    };

    /**
     * @struct AudioCommand
     *
     * @brief Represents a command submitted to the audio thread.
     *
     * This structure encapsulates all information required to perform an
     * operation on an audio source managed by AudioManager.
     *
     * The active field of the data union depends on the selected command
     * type.
     */
    struct AudioCommand {
        /**
         * @brief The command to execute.
         */
        AudioCommandType type;

        /**
         * @brief Identifier of the target audio source.
         */
        uint32_t sourceID;

        /**
         * @union data
         *
         * @brief Payload associated with the command.
         *
         * The active member depends on the command type:
         *
         * - SetPosition → position
         * - SetGain → gain
         * - SetPitch → pitch
         * - SetLooping → looping
         * - SetBuffer → bufferID
         */
        union {
            /**
             * @brief New world-space position of the audio source.
             */
            math::Vector3F position;

            /**
             * @brief Gain multiplier applied to the audio source.
             */
            float gain;

            /**
             * @brief Playback pitch multiplier.
             */
            float pitch;

            /**
             * @brief Looping state of the audio source.
             */
            bool looping;

            /**
             * @brief OpenAL buffer identifier associated with the source.
             */
            ALuint bufferID;
        } data;
    };

    /**
     * @class AudioManager
     *
     * @brief Central manager responsible for OpenAL initialization and audio
     * resource management.
     *
     * AudioManager owns the OpenAL device and context and executes all OpenAL
     * operations on a dedicated audio thread.
     *
     * AudioSource instances communicate with the manager by submitting
     * AudioCommand objects through a thread-safe command queue.
     *
     * This class is responsible for:
     * - Initializing and shutting down OpenAL
     * - Creating and destroying OpenAL sources
     * - Processing asynchronous audio commands
     * - Synchronizing access to shared audio resources
     *
     * @note All OpenAL calls should be performed on the audio thread.
     */
    class AudioManager:
        protected utility::logging::Loggable<
            AudioManager,
            utility::logging::DefaultLogger>
    {
    public:
        /**
         * @brief Creates and initializes the audio subsystem.
         *
         * This constructor opens the default OpenAL device, creates an audio
         * context, and starts the dedicated audio processing thread.
         *
         * @throws std::runtime_error If the audio device cannot be opened or
         * the audio context cannot be created.
         */
        AudioManager();

        /**
         * @brief Stops the audio thread and releases OpenAL resources.
         *
         * This destructor waits for the audio thread to terminate, destroys
         * the OpenAL context, and closes the audio device.
         */
        ~AudioManager();

        /**
         * @brief Submits an audio command for asynchronous execution.
         *
         * Commands are stored in a thread-safe queue and processed by the
         * audio thread.
         *
         * @param command The command to enqueue.
         */
        void submitCommand(const AudioCommand &command);

        /**
         * @brief Creates a new audio source.
         *
         * The corresponding OpenAL source object is created asynchronously on
         * the audio thread.
         *
         * @param buffer Initial audio buffer assigned to the source.
         *
         * @return A unique pointer to the created AudioSource instance.
         */
        std::unique_ptr<AudioSource>
        createAudioSource(std::shared_ptr<AudioBuffer> buffer);

        /**
         * @brief Destroys an existing audio source.
         *
         * @param sourceID Identifier of the source to destroy.
         *
         * @note Consider submitting a DestroySource command instead of
         * directly performing OpenAL operations to ensure all audio API calls
         * remain confined to the audio thread.
         */
        void destroyAudioSource(uint32_t sourceID);

        /**
         * @brief Requests termination of the audio thread.
         */
        void stop();

        /**
         * @brief Indicates whether the audio manager is running.
         *
         * @return true if the audio thread is active, false otherwise.
         */
        bool isRunning() const;

    private:
        /**
         * @brief Main loop executed by the audio thread.
         *
         * The loop processes pending commands until stop() is called.
         */
        void threadLoop();

        /**
         * @brief Processes all pending commands.
         */
        void processCommands();

        /**
         * @brief Executes a single audio command.
         *
         * @param command The command to execute.
         */
        void executeCommand(const AudioCommand &command);

        /**
         * @brief OpenAL device handle.
         */
        ALCdevice *_device = nullptr;

        /**
         * @brief OpenAL context handle.
         */
        ALCcontext *_context = nullptr;

        /**
         * @brief Maps source identifiers to OpenAL source handles.
         */
        std::unordered_map<uint32_t, ALuint> _sources;

        /**
         * @brief Queue containing pending audio commands.
         */
        std::deque<AudioCommand> _commandQueue;

        /**
         * @brief Protects access to the command queue.
         */
        std::mutex _commandQueueMutex;

        /**
         * @brief Dedicated audio processing thread.
         */
        std::thread _audioThread;

        /**
         * @brief Protects access to the source map.
         */
        std::mutex _sourcesMutex;

        /**
         * @brief Identifier assigned to the next created source.
         */
        uint32_t _nextSourceID = 1;

        /**
         * @brief Indicates whether the audio thread should continue running.
         */
        bool _running = true;

        using Loggable::getLogger;
    };
}