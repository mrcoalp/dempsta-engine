#pragma once

#include <string>

#include "Core/core.h"

namespace de {
enum class SoundState { Initial, Playing, Paused, Stopped };

class Sound {
public:
    explicit Sound(std::string filePath) : m_filePath(std::move(filePath)) {}

    static Ref<Sound> CreateSound(const std::string& filePath);

    [[nodiscard]] inline const SoundState& GetState() const { return m_state; }

    virtual void Update() {}

protected:
    SoundState m_state{SoundState::Initial};

private:
    std::string m_filePath;
};
}  // namespace de
