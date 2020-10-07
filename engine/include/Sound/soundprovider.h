#pragma once

#include "Sound/soundapi.h"

namespace de {
class SoundProvider {
public:
    static inline void Init() { s_soundAPI->Init(); }

    static inline void Release() { s_soundAPI->Release(); }

private:
    static Ref<SoundAPI> s_soundAPI;
};
}  // namespace de
