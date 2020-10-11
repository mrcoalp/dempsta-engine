#include "Platform/OpenAL/openalsoundinstance.h"

#define DR_WAV_IMPLEMENTATION
#include <dr_wav/dr_wav.h>

#include "Core/math.h"
#include "Platform/OpenAL/openalutils.h"
#include "Sound/soundprovider.h"
#include "Utils/fileutils.h"

namespace de {
/// Define max number of buffers to use when streaming
constexpr size_t MAX_BUFFERS{4};
/// Define each buffer size: 64kb
constexpr size_t BUFFER_SIZE{64 * 1024};
/// Max size to only use a buffer per instance
constexpr size_t MAX_BUFFER_SIZE{MAX_BUFFERS * BUFFER_SIZE};

static ALenum GetFormat(uint8_t channels, uint8_t bitsPerSample) {
    const bool stereo = channels > 1;
    switch (bitsPerSample) {
        case 8:
            return stereo ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
        case 16:
            return stereo ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
        default:
            return AL_NONE;
    }
}

OpenALSoundInstance::OpenALSoundInstance(const std::string& filePath) {
    if (loadFile(filePath)) {
        m_stream = m_soundData.size() > MAX_BUFFER_SIZE;

        if (m_stream) {
            m_buffers.resize(MAX_BUFFERS);
            AL_CALL(alGenBuffers, MAX_BUFFERS, m_buffers.data());
        } else {  // No need to stream, use only one buffer and set its data.
            m_buffers.resize(1);
            AL_CALL(alGenBuffers, 1, m_buffers.data());
            AL_CALL(alBufferData, m_buffers[0], m_format, m_soundData.data(), m_soundData.size(), m_info.sampleRate);
        }

        SoundProvider::DestroyInstanceCallback([this]() { Unload(); });
    }
}

bool OpenALSoundInstance::loadFile(const std::string& filePath) {
    const auto extension = FileUtils::GetFileExtension(filePath);
    if (extension == "wav") {
        drwav wav;
        if (drwav_init_file(&wav, filePath.c_str(), nullptr) == 0) {
            LOG_ENGINE_ERROR("Unable to read audio file: {} - unsupported wav format", filePath);
            drwav_uninit(&wav);
            return false;
        }
        m_soundData.resize(wav.dataChunkDataSize);
        size_t framesRead = drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, m_soundData.data());
        if (m_soundData.empty() || framesRead != wav.totalPCMFrameCount) {
            LOG_ENGINE_ERROR("Unable to read audio file: {} - empty wav data", filePath);
            drwav_uninit(&wav);
            return false;
        }
        m_info.channels = wav.channels;
        m_info.sampleRate = wav.sampleRate;
        m_info.bitsPerSample = wav.bitsPerSample;
        m_format = GetFormat(m_info.channels, m_info.bitsPerSample);
        m_loadedData = true;
        drwav_uninit(&wav);
    } else {
        LOG_ENGINE_ERROR("Unable to read audio file: {} - unsupported format \"{}\"! Only wav files are supported.", filePath, extension);
        return false;
    }
    return true;
}

void OpenALSoundInstance::updateStream() {
    ALint buffersProcessed = 0;
    AL_CALL(alGetSourcei, m_source, AL_BUFFERS_PROCESSED, &buffersProcessed);

    while (buffersProcessed-- > 0) {
        ALuint buffer = 0;
        AL_CALL(alSourceUnqueueBuffers, m_source, 1, &buffer);

        ALsizei size = m_cursor + BUFFER_SIZE > m_soundData.size() ? m_soundData.size() - m_cursor : BUFFER_SIZE;

        if (size > 0) {
            AL_CALL(alBufferData, buffer, m_format, &m_soundData[m_cursor], size, m_info.sampleRate);
            AL_CALL(alSourceQueueBuffers, m_source, 1, &buffer);
            m_cursor += size;
        }
    }
}

void OpenALSoundInstance::Update() {
    if (m_hasSource) {
        if (m_stream) {
            updateStream();
        }
        ALint state = AL_PLAYING;
        AL_CALL(alGetSourcei, m_source, AL_SOURCE_STATE, &state);
        if (m_state == SoundState::Playing && state != AL_PLAYING) {
            if (!m_stream) {  // When not streaming, we can stop the instance right here
                Stop();
                return;
            }
            // We need to handle the case where, for some reason, we stopped feeding new buffers to OpenAL
            // however the instance has not yet finished
            if (m_cursor >= m_soundData.size()) {  // If cursor has reached the end, play or stop instance based on lopping state
                GetLooped() ? Play() : Stop();     // Looping is handled manually when streaming
            } else {
                AL_CALL(alSourcePlay, m_source);  // The instance is not finished yet, continue playing
            }
        }
    }
}

void OpenALSoundInstance::Play() {
    if (!m_loadedData) {
        LOG_ENGINE_WARN("Tried to play a sound instance without data!");
        return;
    }
    Stop();  // Ensure instance is stopped when trying to play.
    if (SoundProvider::CheckAndGetAvailableSource(m_source)) {
        AL_CALL(alSourcef, m_source, AL_GAIN, m_info.gain);
        AL_CALL(alSourcef, m_source, AL_PITCH, m_info.pitch);
        AL_CALL(alSource3f, m_source, AL_POSITION, m_info.pan, 0, 0);
        AL_CALL(alSource3f, m_source, AL_VELOCITY, 0, 0, 0);
        if (m_stream) {                                 // Start streaming to OpenAL
            for (size_t i = 0; i < MAX_BUFFERS; ++i) {  // Queue all buffers
                AL_CALL(alBufferData, m_buffers[i], m_format, &m_soundData[i * BUFFER_SIZE], BUFFER_SIZE, m_info.sampleRate);
            }
            AL_CALL(alSourceQueueBuffers, m_source, MAX_BUFFERS, m_buffers.data());
            m_cursor = MAX_BUFFERS * BUFFER_SIZE;  // Move cursor accordingly
        } else {
            // Only set looping when not streaming, otherwise we would loop a buffer with part of the audio and not its entirety
            AL_CALL(alSourcei, m_source, AL_LOOPING, m_info.looped ? AL_TRUE : AL_FALSE);
            AL_CALL(alSourcei, m_source, AL_BUFFER, m_buffers[0]);
        }

        AL_CALL(alSourcePlay, m_source);
        m_state = SoundState::Playing;
        m_hasSource = true;
    }
}

void OpenALSoundInstance::Pause() {
    if (!m_loadedData) {
        LOG_ENGINE_WARN("Tried to pause a sound instance without data!");
        return;
    }
    if (!m_hasSource || m_state != SoundState::Playing || m_state == SoundState::Paused) {
        return;
    }
    AL_CALL(alSourcePause, m_source);
    m_state = SoundState::Paused;
}

void OpenALSoundInstance::Resume() {
    if (!m_loadedData) {
        LOG_ENGINE_WARN("Tried to resume a sound instance without data!");
        return;
    }
    if (!m_hasSource) {
        Play();
        return;
    }
    if (m_state == SoundState::Paused) {
        AL_CALL(alSourcePlay, m_source);
        m_state = SoundState::Playing;
    }
}

void OpenALSoundInstance::Stop() {
    if (!m_loadedData) {
        LOG_ENGINE_WARN("Tried to stop a sound instance without data!");
        return;
    }
    if (m_hasSource) {
        AL_CALL(alSourceStop, m_source);
        AL_CALL(alSourcei, m_source, AL_BUFFER, AL_NONE);
        SoundProvider::ReleaseSource(m_source);
        m_hasSource = false;
    }
    m_state = SoundState::Stopped;
}

void OpenALSoundInstance::SetGain(float gain) {
    m_info.gain = gain >= 0.f && gain <= 1.f ? gain : Math::Clamp(gain, 0.f, 1.f);
    if (m_hasSource) {
        AL_CALL(alSourcef, m_source, AL_GAIN, m_info.gain);
    }
}

void OpenALSoundInstance::SetPitch(float pitch) {
    m_info.pitch = pitch >= 0.5f && pitch <= 2.f ? pitch : Math::Clamp(pitch, 0.5f, 2.f);
    if (m_hasSource) {
        AL_CALL(alSourcef, m_source, AL_PITCH, m_info.pitch);
    }
}

void OpenALSoundInstance::SetPan(float pan) {
    m_info.pan = pan >= -1.f && pan <= 1.f ? pan : Math::Clamp(pan, -1.f, 1.f);
    if (m_hasSource) {
        AL_CALL(alSource3f, m_source, AL_POSITION, m_info.pan, 0, 0);
    }
}

void OpenALSoundInstance::SetLooped(bool looped) {
    m_info.looped = looped;
    // Only set looping when not streaming, otherwise we would loop a buffer with part of the audio and not its entirety
    if (m_hasSource && !m_stream) {
        AL_CALL(alSourcei, m_source, AL_LOOPING, m_info.looped ? AL_TRUE : AL_FALSE);
    }
}

void OpenALSoundInstance::Unload() { AL_CALL(alDeleteBuffers, m_stream ? MAX_BUFFERS : 1, m_buffers.data()); }
}  // namespace de
