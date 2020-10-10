#pragma once

#include <string>

#include "Core/core.h"

namespace de {
/**
 * @brief Represents the state of the sound instance.
 */
enum class SoundState { Initial, Playing, Paused, Stopped };

/**
 * @brief Holds all relevant audio data. To be defined in sound api implementation.
 */
struct AudioData;

class Sound {
public:
    /**
     * @brief Runs every frame. Updates the state of sound.
     */
    virtual void Update() = 0;

    /**
     * @brief Plays the sound.
     */
    virtual void Play() = 0;

    /**
     * @brief Pauses the sound, if playing.
     */
    virtual void Pause() = 0;

    /**
     * @brief Resumes the sound, if paused.
     */
    virtual void Resume() = 0;

    /**
     * @brief Stops the sound, if playing.
     */
    virtual void Stop() = 0;

    /**
     * @brief Unloads sound from memory.
     */
    virtual void Unload() = 0;

    [[nodiscard]] virtual float GetGain() const = 0;

    virtual void SetGain(float gain) = 0;

    [[nodiscard]] virtual float GetPitch() const = 0;

    virtual void SetPitch(float pitch) = 0;

    [[nodiscard]] virtual float GetPan() const = 0;

    virtual void SetPan(float pan) = 0;

    [[nodiscard]] virtual bool GetLooped() const = 0;

    virtual void SetLooped(bool looped) = 0;

    /**
     * @brief Whether or not the sound is playing.
     *
     * @return true
     * @return false
     */
    [[nodiscard]] virtual bool IsPlaying() const = 0;

    /**
     * @brief Get the Audio Data ref object. To directly tweak properties.
     *
     * @return AudioData&
     */
    [[nodiscard]] virtual AudioData& GetAudioData() = 0;
};

class SoundInstance : public Sound {
public:
    /**
     * @brief Create a SoundInstance object, based on platform API.
     *
     * @param filePath Path to the source sound file.
     * @return Ref<SoundInstance>
     */
    static Ref<SoundInstance> CreateSound(const std::string& filePath);

    [[nodiscard]] inline bool IsPlaying() const final { return m_state == SoundState::Playing; }

protected:
    SoundState m_state{SoundState::Initial};
};
}  // namespace de
