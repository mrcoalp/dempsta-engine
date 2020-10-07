#pragma once

#include "Sound/sound.h"

namespace de {
class OpenALSound : public Sound {
public:
    explicit OpenALSound(std::string filePath) : Sound(std::move(filePath)) {}

    void Update() final {}
};
}  // namespace de
