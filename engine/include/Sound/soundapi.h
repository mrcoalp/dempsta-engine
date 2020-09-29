#pragma once

#include "Core/core.h"

namespace de {
class SoundAPI {
public:
    virtual ~SoundAPI() = default;

    virtual void Init() = 0;
};
}  // namespace de
