#pragma once

#include "Sound/soundapi.h"

namespace de {
class OpenALSoundAPI : public SoundAPI {
public:
    OpenALSoundAPI() = default;

    void Init() final;
};
}  // namespace de
