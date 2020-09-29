#pragma once

#include "Sound/soundapi.h"

struct ALCdevice;
struct ALCcontext;

namespace de {
class OpenALSoundAPI : public SoundAPI {
public:
    OpenALSoundAPI();

    void Init() final;

private:
    struct Internal {
        ALCdevice* device;
        ALCcontext* context;
    };

    Scope<Internal> m_internal;
};
}  // namespace de
