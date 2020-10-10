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

    AL_CALL(alGenBuffers, 1, &m_buffer);

    std::vector<uint8_t> soundData;
    soundData.resize(wav.dataChunkDataSize);
    size_t framesRead = drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, soundData.data());
    if (soundData.empty() || framesRead != wav.totalPCMFrameCount) {
        LOG_ENGINE_ERROR("Unable to read audio file: {} - empty wav data", filePath);
        drwav_uninit(&wav);
        return;
    }

    AL_CALL(alBufferData, m_buffer, m_format, soundData.data(), soundData.size(), m_data.sampleRate);

    drwav_uninit(&wav);

    SoundProvider::DestroyInstanceCallback([this]() { Unload(); });
}

void OpenALSoundInstance::Update() {
    if (m_hasSource) {
        ALint state;
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
        AL_CALL(alSourcei, m_source, AL_BUFFER, m_buffer);

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

void OpenALSoundInstance::Unload() { AL_CALL(alDeleteBuffers, 1, &m_buffer); }
}  // namespace de
