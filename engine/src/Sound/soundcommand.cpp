#include "Sound/soundcommand.h"

#include "Platform/OpenAL/openalsoundapi.h"

namespace de {
Ref<SoundAPI> SoundCommand::s_soundAPI = CreateRef<OpenALSoundAPI>();
}
