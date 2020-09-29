#pragma once

#include "Sound/soundcommand.h"

namespace de {
class SoundProvider {
public:
    static inline void Init() { SoundCommand::Init(); }
};
}  // namespace de
