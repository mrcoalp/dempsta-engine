#pragma once

#include "Core/core.h"
#include "Sound/soundapi.h"

namespace de {
class SoundCommand {
public:
    static inline void Init() { s_soundAPI->Init(); }

private:
    static Ref<SoundAPI> s_soundAPI;
};
}  // namespace de
