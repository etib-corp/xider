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

#include "utility/sound/audio_device.hpp"

utility::sound::AudioDevice::AudioDevice()
{
    _device = alcOpenDevice(nullptr);
    if (!_device) {
        getLogger().error() << "Failed to open audio device";
        return;
    }

    _context = alcCreateContext(_device, nullptr);
    if (!_context) {
        alcCloseDevice(_device);
        getLogger().error() << "Failed to create audio context";
        return;
    }

    if (!alcMakeContextCurrent(_context)) {
        alcDestroyContext(_context);
        alcCloseDevice(_device);
        getLogger().error() << "Failed to make audio context current";
        return;
    }
}

utility::sound::AudioDevice::~AudioDevice()
{
    alcMakeContextCurrent(nullptr);
    if (_context) {
        alcDestroyContext(_context);
    }
    if (_device) {
        alcCloseDevice(_device);
    }
}