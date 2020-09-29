#include "Platform/OpenAL/openalsoundapi.h"

#include <AL/alc.h>

#include "Core/core.h"
#include "Platform/OpenAL/openalutils.h"

namespace de {
void OpenALSoundAPI::Init() {
    ALCdevice* openALDevice = alcOpenDevice(nullptr);
    DE_ASSERT(openALDevice != nullptr, "Could not open ALC device!")
    ALCcontext* openALContext;
    DE_ASSERT(ALC_CALL(alcCreateContext, openALContext, openALDevice, openALDevice, nullptr) && openALContext != nullptr,
              "Could not create audio context!")
    ALCboolean contextMadeCurrent = ALC_FALSE;
    DE_ASSERT(ALC_CALL(alcMakeContextCurrent, contextMadeCurrent, openALDevice, openALContext) && contextMadeCurrent == ALC_TRUE,
              "Could not make audio context current!")
}
}  // namespace de
