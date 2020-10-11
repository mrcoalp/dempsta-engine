#include "Sound/soundprovider.h"

#include "Platform/OpenAL/openalsoundapi.h"

namespace de {
Ref<SoundAPI> SoundProvider::s_soundAPI = CreateRef<OpenALSoundAPI>();
}
