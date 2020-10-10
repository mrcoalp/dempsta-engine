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

    [[nodiscard]] inline float GetGain() const final { return m_data.gain; }
    void SetGain(float gain) final;

    [[nodiscard]] inline float GetPitch() const final { return m_data.pitch; }
    void SetPitch(float pitch) final;

    [[nodiscard]] inline float GetPan() const final { return m_data.pan; }
    void SetPan(float pan) final;

    [[nodiscard]] inline bool GetLooped() const final { return m_data.looped; }
    void SetLooped(bool looped) final;

    [[nodiscard]] inline const AudioData& GetAudioData() const final { return m_data; }

private:
    bool m_hasSource{false};
    ALuint m_buffer;
    ALuint m_source;
    ALenum m_format;

    AudioData m_data;
};
}  // namespace de
