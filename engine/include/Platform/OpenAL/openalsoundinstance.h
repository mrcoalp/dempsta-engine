#pragma once

#include <AL/al.h>

#include "Sound/sound.h"

namespace de {
class OpenALSoundInstance : public SoundInstance {
public:
    explicit OpenALSoundInstance(const std::string& filePath);

    void Update() final;

    void Play() final;

    void Pause() final;

    void Resume() final;

    void Stop() final;

    void Unload() final;

private:
    ALuint m_buffer;
    ALuint m_source;
    ALenum m_format;
    uint8_t m_channels;
    int32_t m_sampleRate;
    uint8_t m_bitsPerSample;
};
}  // namespace de
