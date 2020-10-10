#pragma once

#include "Sound/soundapi.h"

struct ALCdevice;
struct ALCcontext;

namespace de {
class OpenALSoundAPI : public SoundAPI {
public:
    OpenALSoundAPI();

    void Init() final;

    void Destroy() final;

    bool CheckAndGetAvailableSource(uint32_t& source) final;

    void ReleaseSource(uint32_t source) final;

private:
    struct Internal {
        ALCdevice* device;
        ALCcontext* context;
    };

    Scope<Internal> m_internal;

    std::vector<uint32_t> m_availableSources;
    std::vector<uint32_t> m_sourcesInUse;
    std::vector<uint32_t> m_allSources;
};
}  // namespace de
