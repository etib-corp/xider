#pragma once

#include "utility/sound/decoder/audio_decoder.hpp"

namespace utility::sound
{
    /**
     * @class WAVDecoder
     *
     * @brief Decoder implementation for Waveform Audio File Format (WAV)
     * files.
     *
     * WAVDecoder uses the dr_wav library to decode WAV-encoded audio data
     * into raw Pulse-Code Modulation (PCM) samples compatible with OpenAL.
     *
     * The decoder supports uncompressed PCM WAV files and uploads the
     * decoded samples to an AudioBuffer instance for playback.
     */
    class WAVDecoder : public AAudioDecoder
    {
    public:
        /**
         * @brief Creates a WAV decoder instance.
         */
        WAVDecoder();

        /**
         * @brief Destroys the WAV decoder instance.
         */
        ~WAVDecoder() override;

        /**
         * @brief Determines whether the specified file can be decoded.
         *
         * Compatibility is determined from the file extension.
         *
         * @param filePath Path to the audio file.
         *
         * @return true if the file extension is supported, false otherwise.
         */
        bool canDecode(const std::string &filePath) const override;

        /**
         * @brief Determines whether the specified decoder type is supported.
         *
         * @param type Audio decoder type to test.
         *
         * @return true if the decoder supports the specified type, false
         * otherwise.
         */
        bool canDecode(AudioDecoderType type) const override;

        /**
         * @brief Decodes a WAV file into an AudioBuffer.
         *
         * The encoded audio stream is decoded into PCM samples and uploaded
         * to an AudioBuffer for playback.
         *
         * @param file The file containing WAV-encoded audio data.
         *
         * @return A shared pointer to the decoded AudioBuffer, or nullptr if
         * decoding fails.
         */
        [[nodiscard]]
        std::shared_ptr<AudioBuffer>
        decode(std::shared_ptr<utility::File> file) override;
    };
}