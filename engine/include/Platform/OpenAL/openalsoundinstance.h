#pragma once

#include <AL/al.h>

#include "Sound/sound.h"

namespace de {
constexpr std::size_t NUM_BUFFERS{4};
constexpr std::size_t BUFFER_SIZE{65536};  // 32kb of data in each buffer

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
    void updateStream();

    bool m_hasSource{false};
    bool m_stream{false};
    size_t m_cursor{0};
    std::vector<ALuint> m_buffers;
    std::vector<uint8_t> m_soundData;
    ALuint m_source;
    ALenum m_format;

    AudioData m_data;
};
}  // namespace de
