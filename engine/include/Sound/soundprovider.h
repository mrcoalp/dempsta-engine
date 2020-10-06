#pragma once

#include "Sound/soundcommand.h"

namespace de {
class SoundProvider {
public:
    static inline void Init() { SoundCommand::Init(); }

    static inline void Release() { SoundCommand::Release(); }
};
}  // namespace de
