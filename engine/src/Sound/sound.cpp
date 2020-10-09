#include "Sound/sound.h"

#include "Platform/OpenAL/openalsoundinstance.h"
#include "Sound/soundapi.h"

namespace de {
Ref<SoundInstance> SoundInstance::CreateSound(const std::string& filePath) {
    switch (SoundAPI::GetAPI()) {
        case SoundAPI::API::None:
            return {};
        case SoundAPI::API::OpenAL:
            return CreateRef<OpenALSoundInstance>(filePath);
        default:
            return {};
    }
}
}  // namespace de
