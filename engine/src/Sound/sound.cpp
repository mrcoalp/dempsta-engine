#include "Sound/sound.h"

#include "Platform/OpenAL/openalsound.h"
#include "Sound/soundapi.h"

namespace de {
Ref<Sound> Sound::CreateSound(const std::string& filePath) {
    switch (SoundAPI::GetAPI()) {
        case SoundAPI::API::None:
            return {};
        case SoundAPI::API::OpenAL:
            return CreateRef<OpenALSound>(filePath);
        default:
            return {};
    }
}
}  // namespace de
