#include "Platform/OpenAL/openalsoundinstance.h"

#include <dr_wav/dr_wav.h>

#include "Platform/OpenAL/openalutils.h"

namespace de {
OpenALSoundInstance::OpenALSoundInstance(const std::string& filePath) {
    drwav wav;
    if (drwav_init_file(&wav, filePath.c_str(), nullptr) == 0) {
        LOG_ENGINE_ERROR("Unable to read audio file: {} - unsupported format", filePath);
        drwav_uninit(&wav);
        return;
    }
    m_channels = wav.channels;
    m_sampleRate = wav.sampleRate;
    m_bitsPerSample = wav.bitsPerSample;

    AL_CALL(alGenBuffers, 1, &m_buffer);

    if (m_channels == 1 && m_bitsPerSample == 8) {
        m_format = AL_FORMAT_MONO8;
    } else if (m_channels == 1 && m_bitsPerSample == 16) {
        m_format = AL_FORMAT_MONO16;
    } else if (m_channels == 2 && m_bitsPerSample == 8) {
        m_format = AL_FORMAT_STEREO8;
    } else if (m_channels == 2 && m_bitsPerSample == 16) {
        m_format = AL_FORMAT_STEREO16;
    }

    std::vector<uint8_t> soundData;
    soundData.resize(wav.dataChunkDataSize);
    size_t framesRead = drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, soundData.data());
    if (soundData.empty() || framesRead != wav.totalPCMFrameCount) {
        LOG_ENGINE_ERROR("Unable to read audio file: {} - empty wav data", filePath);
        drwav_uninit(&wav);
        return;
    }

    AL_CALL(alBufferData, m_buffer, m_format, soundData.data(), soundData.size(), m_sampleRate);

    AL_CALL(alGenSources, 1, &m_source);
    AL_CALL(alSourcef, m_source, AL_PITCH, 1);
    AL_CALL(alSourcef, m_source, AL_GAIN, 1.0f);
    AL_CALL(alSource3f, m_source, AL_POSITION, 0, 0, 0);
    AL_CALL(alSource3f, m_source, AL_VELOCITY, 0, 0, 0);
    AL_CALL(alSourcei, m_source, AL_LOOPING, AL_FALSE);
    AL_CALL(alSourcei, m_source, AL_BUFFER, m_buffer);

    drwav_uninit(&wav);
}

void OpenALSoundInstance::Update() {
    if (m_state == SoundState::Playing) {
        ALint state;
        AL_CALL(alGetSourcei, m_source, AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING) {
            Stop();
        }
    }
}

void OpenALSoundInstance::Play() {
    AL_CALL(alSourcePlay, m_source);
    m_state = SoundState::Playing;
}

void OpenALSoundInstance::Pause() {
    // TODO(marco)
}

void OpenALSoundInstance::Resume() {
    // TODO(marco)
}

void OpenALSoundInstance::Stop() {
    alSourceStop(m_source);
    alSourcei(m_source, AL_BUFFER, 0);
    m_state = SoundState::Stopped;
}

void OpenALSoundInstance::Unload() {
    AL_CALL(alDeleteSources, 1, &m_source);
    AL_CALL(alDeleteBuffers, 1, &m_buffer);
}
}  // namespace de
