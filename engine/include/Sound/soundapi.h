#pragma once

#include "Core/core.h"

namespace de {
class SoundAPI {
public:
    virtual ~SoundAPI() = default;

    enum class API { None, OpenAL };

    virtual void Init() = 0;

    virtual void Release() = 0;

    [[nodiscard]] static inline API GetAPI() { return s_api; }

private:
    static API s_api;
};
}  // namespace de
