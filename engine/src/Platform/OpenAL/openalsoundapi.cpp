#include "Platform/OpenAL/openalsoundapi.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "Platform/OpenAL/openalutils.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav/dr_wav.h"

namespace de {
OpenALSoundAPI::OpenALSoundAPI() : m_internal(CreateScope<Internal>()) {}

void OpenALSoundAPI::Init() {
    m_internal->device = alcOpenDevice(nullptr);
    DE_ASSERT(m_internal->device != nullptr, "Could not open ALC device!")
    DE_ASSERT(ALC_CALL(alcCreateContext, m_internal->context, m_internal->device, m_internal->device, nullptr) && m_internal->context != nullptr,
              "Could not create audio context!")
    ALCboolean contextMadeCurrent = ALC_FALSE;
    DE_ASSERT(ALC_CALL(alcMakeContextCurrent, contextMadeCurrent, m_internal->device, m_internal->context) && contextMadeCurrent == ALC_TRUE,
              "Could not make audio context current!")

    drwav wav;

    drwav_init_file(&wav, "assets/sound/iamtheprotectorofthissystem.wav", nullptr);
    uint8_t channels = wav.channels;
    int32_t sampleRate = wav.sampleRate;
    uint8_t bitsPerSample = wav.bitsPerSample;

    ALuint buffer;
    AL_CALL(alGenBuffers, 1, &buffer);

    ALenum format = 0;
    if (channels == 1 && bitsPerSample == 8)
        format = AL_FORMAT_MONO8;
    else if (channels == 1 && bitsPerSample == 16)
        format = AL_FORMAT_MONO16;
    else if (channels == 2 && bitsPerSample == 8)
        format = AL_FORMAT_STEREO8;
    else if (channels == 2 && bitsPerSample == 16)
        format = AL_FORMAT_STEREO16;

    std::vector<uint8_t> soundData;
    soundData.resize(wav.dataChunkDataSize);
    size_t framesread = drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, soundData.data());

    AL_CALL(alBufferData, buffer, format, soundData.data(), soundData.size(), sampleRate);

    ALuint source;
    AL_CALL(alGenSources, 1, &source);
    AL_CALL(alSourcef, source, AL_PITCH, 1);
    AL_CALL(alSourcef, source, AL_GAIN, 1.0f);
    AL_CALL(alSource3f, source, AL_POSITION, 0, 0, 0);
    AL_CALL(alSource3f, source, AL_VELOCITY, 0, 0, 0);
    AL_CALL(alSourcei, source, AL_LOOPING, AL_FALSE);
    AL_CALL(alSourcei, source, AL_BUFFER, buffer);

    AL_CALL(alSourcePlay, source);

    ALint state = AL_PLAYING;

    while (state == AL_PLAYING) {
        AL_CALL(alGetSourcei, source, AL_SOURCE_STATE, &state);
    }

    AL_CALL(alDeleteSources, 1, &source);
    AL_CALL(alDeleteBuffers, 1, &buffer);
    drwav_uninit(&wav);
}

void OpenALSoundAPI::Release() {
    ALCboolean contextMadeCurrent = ALC_TRUE;
    ALC_CALL(alcMakeContextCurrent, contextMadeCurrent, m_internal->device, nullptr);
    ALC_CALL(alcDestroyContext, m_internal->device, m_internal->context);
    ALCboolean closed;
    ALC_CALL(alcCloseDevice, closed, m_internal->device, m_internal->device);
}
}  // namespace de
