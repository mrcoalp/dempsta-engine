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

    [[nodiscard]] inline float GetGain() const final { return m_info.gain; }
    void SetGain(float gain) final;

    [[nodiscard]] inline float GetPitch() const final { return m_info.pitch; }
    void SetPitch(float pitch) final;

    [[nodiscard]] inline float GetPan() const final { return m_info.pan; }
    void SetPan(float pan) final;

    [[nodiscard]] inline bool GetLooped() const final { return m_info.looped; }
    void SetLooped(bool looped) final;

    [[nodiscard]] inline const AudioInfo& GetAudioInfo() const final { return m_info; }

private:
    /**
     * @brief Loads a sound file.
     *
     * @param filePath Path to file.
     * @return true File loaded with success.
     * @return false Failed to load file.
     */
    bool loadFile(const std::string& filePath);

    /**
     * @brief Updates stream, dequeue used and queuing new buffers.
     */
    void updateStream();

    /// Whether or not the instance has a valid source fetched from provider.
    bool m_hasSource{false};
    /// Whether or not the instance is being streamed. Defined by how big audio data is.
    bool m_stream{false};
    /// Current "needle position in record". Used to keep track of already streamed data, when streaming.
    size_t m_cursor{0};
    /// Buffers to be used.
    std::vector<ALuint> m_buffers;
    /// Audio data from audio file source.
    std::vector<uint8_t> m_soundData;
    /// Whether or not the sound data was loaded with success.
    bool m_loadedData{false};
    /// OpenAL source.
    ALuint m_source;
    /// Audio format used by OpenAL.
    ALenum m_format;
    /// Struct containing audio properties info.
    AudioInfo m_info;
};
}  // namespace de
