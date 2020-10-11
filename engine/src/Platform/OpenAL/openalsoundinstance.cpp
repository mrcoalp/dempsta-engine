#include "Platform/OpenAL/openalsoundinstance.h"

#include <dr_wav/dr_wav.h>

#include "Core/math.h"
#include "Platform/OpenAL/openalutils.h"
#include "Sound/soundprovider.h"

namespace de {
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
    drwav wav;
    if (drwav_init_file(&wav, filePath.c_str(), nullptr) == 0) {
        LOG_ENGINE_ERROR("Unable to read audio file: {} - unsupported format", filePath);
        drwav_uninit(&wav);
        return;
    }
    m_data.channels = wav.channels;
    m_data.sampleRate = wav.sampleRate;
    m_data.bitsPerSample = wav.bitsPerSample;
    m_format = GetFormat(m_data.channels, m_data.bitsPerSample);

    m_soundData.resize(wav.dataChunkDataSize);
    size_t framesRead = drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, m_soundData.data());
    if (m_soundData.empty() || framesRead != wav.totalPCMFrameCount) {
        LOG_ENGINE_ERROR("Unable to read audio file: {} - empty wav data", filePath);
        drwav_uninit(&wav);
        return;
    }

    m_stream = m_soundData.size() > NUM_BUFFERS * BUFFER_SIZE;

    if (m_stream) {
        m_allBuffers.resize(NUM_BUFFERS);
        for (auto& buffer : m_allBuffers) {
            AL_CALL(alGenBuffers, 1, &buffer);
        }
        m_freeBuffers = m_allBuffers;
    } else {
        m_allBuffers.resize(1);
        AL_CALL(alGenBuffers, 1, &m_allBuffers[0]);
        AL_CALL(alBufferData, m_allBuffers[0], m_format, m_soundData.data(), m_soundData.size(), m_data.sampleRate);
    }

    drwav_uninit(&wav);

    SoundProvider::DestroyInstanceCallback([this]() { Unload(); });
}

void OpenALSoundInstance::updateStream() {
    ALint buffersProcessed = 0;
    AL_CALL(alGetSourcei, m_source, AL_BUFFERS_PROCESSED, &buffersProcessed);

    while (buffersProcessed-- > 0) {
        ALuint buffer = 0;
        AL_CALL(alSourceUnqueueBuffers, m_source, 1, &buffer);
        m_freeBuffers.push_back(buffer);

        ALsizei size = m_cursor + BUFFER_SIZE > m_soundData.size() ? m_soundData.size() - m_cursor : BUFFER_SIZE;

        if (size > 0) {
            AL_CALL(alBufferData, buffer, m_format, &m_soundData[m_cursor], size, m_data.sampleRate);
            AL_CALL(alSourceQueueBuffers, m_source, 1, &buffer);
            m_cursor += size;
        }
    }
}

void OpenALSoundInstance::Update() {
    if (m_hasSource) {
        ALint state;
        if (m_stream) {
            updateStream();
        }
        AL_CALL(alGetSourcei, m_source, AL_SOURCE_STATE, &state);
        if (m_state == SoundState::Playing && state != AL_PLAYING) {
            Stop();
        }
    }
}

void OpenALSoundInstance::Play() {
    Stop();
    if (SoundProvider::CheckAndGetAvailableSource(m_source)) {
        AL_CALL(alSourcef, m_source, AL_GAIN, m_data.gain);
        AL_CALL(alSourcef, m_source, AL_PITCH, m_data.pitch);
        AL_CALL(alSource3f, m_source, AL_POSITION, m_data.pan, 0, 0);
        AL_CALL(alSourcei, m_source, AL_LOOPING, m_data.looped ? AL_TRUE : AL_FALSE);
        AL_CALL(alSource3f, m_source, AL_VELOCITY, 0, 0, 0);
        if (m_stream) {
            size_t index = 0;
            for (auto& buffer : m_freeBuffers) {
                AL_CALL(alBufferData, buffer, m_format, &m_soundData[index * BUFFER_SIZE], BUFFER_SIZE, m_data.sampleRate);
                AL_CALL(alSourceQueueBuffers, m_source, 1, &buffer);
                ++index;
            }
            m_freeBuffers.clear();
            m_cursor = NUM_BUFFERS * BUFFER_SIZE;
        } else {
            AL_CALL(alSourcei, m_source, AL_BUFFER, m_allBuffers[0]);
        }

        AL_CALL(alSourcePlay, m_source);
        m_state = SoundState::Playing;
        m_hasSource = true;
    }
}

void OpenALSoundInstance::Pause() {
    if (!m_hasSource || m_state != SoundState::Playing || m_state == SoundState::Paused) {
        return;
    }
    AL_CALL(alSourcePause, m_source);
    m_state = SoundState::Paused;
}

void OpenALSoundInstance::Resume() {
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
    if (m_hasSource) {
        AL_CALL(alSourceStop, m_source);
        AL_CALL(alSourcei, m_source, AL_BUFFER, AL_NONE);
        m_freeBuffers = m_allBuffers;
        SoundProvider::ReleaseSource(m_source);
        m_hasSource = false;
    }
    m_state = SoundState::Stopped;
}

void OpenALSoundInstance::SetGain(float gain) {
    m_data.gain = gain >= 0.f && gain <= 1.f ? gain : Math::Clamp(gain, 0.f, 1.f);
    if (m_hasSource) {
        AL_CALL(alSourcef, m_source, AL_GAIN, m_data.gain);
    }
}

void OpenALSoundInstance::SetPitch(float pitch) {
    m_data.pitch = pitch >= 0.5f && pitch <= 2.f ? pitch : Math::Clamp(pitch, 0.5f, 2.f);
    if (m_hasSource) {
        AL_CALL(alSourcef, m_source, AL_PITCH, m_data.pitch);
    }
}

void OpenALSoundInstance::SetPan(float pan) {
    m_data.pan = pan >= -1.f && pan <= 1.f ? pan : Math::Clamp(pan, -1.f, 1.f);
    if (m_hasSource) {
        AL_CALL(alSource3f, m_source, AL_POSITION, m_data.pan, 0, 0);
    }
}

void OpenALSoundInstance::SetLooped(bool looped) {
    m_data.looped = looped;
    if (m_hasSource) {
        AL_CALL(alSourcei, m_source, AL_LOOPING, m_data.looped ? AL_TRUE : AL_FALSE);
    }
}

void OpenALSoundInstance::Unload() { AL_CALL(alDeleteBuffers, m_stream ? NUM_BUFFERS : 1, m_allBuffers.data()); }
}  // namespace de
