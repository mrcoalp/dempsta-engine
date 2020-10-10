#pragma once

#include "Sound/soundapi.h"

namespace de {
class SoundProvider {
public:
    /**
     * @brief Run sound provider initialization.
     */
    static inline void Init() { s_soundAPI->Init(); }

    /**
     * @brief Destroy sound provider and handle initialized devices.
     */
    static inline void Destroy() { s_soundAPI->Destroy(); }

    /**
     * @brief Checks for available sources to be used in a sound instance. If there are any, source gets populated.
     *
     * @param source Source id to be populated.
     * @return true If there are available sources.
     * @return false Otherwise.
     */
    static inline bool CheckAndGetAvailableSource(uint32_t& source) { return s_soundAPI->CheckAndGetAvailableSource(source); }

    /**
     * @brief Releases passed source id.
     *
     * @param source Source id to be released.
     */
    static inline void ReleaseSource(uint32_t source) { s_soundAPI->ReleaseSource(source); }

private:
    static Ref<SoundAPI> s_soundAPI;
};
}  // namespace de
