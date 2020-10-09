#include "Platform/OpenAL/openalsoundapi.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "Platform/OpenAL/openalutils.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav/dr_wav.h"

namespace de {
OpenALSoundAPI::OpenALSoundAPI() : m_internal(CreateScope<Internal>()) {}

void OpenALSoundAPI::Init() {
    m_internal->device = alcOpenDevice(nullptr);
    DE_ASSERT(m_internal->device != nullptr, "Could not open ALC device!")
    DE_ASSERT(ALC_CALL(alcCreateContext, m_internal->context, m_internal->device, m_internal->device, nullptr) && m_internal->context != nullptr,
              "Could not create audio context!")
    ALCboolean contextMadeCurrent = ALC_FALSE;
    DE_ASSERT(ALC_CALL(alcMakeContextCurrent, contextMadeCurrent, m_internal->device, m_internal->context) && contextMadeCurrent == ALC_TRUE,
              "Could not make audio context current!")
}

void OpenALSoundAPI::Release() {
    ALCboolean contextMadeCurrent = ALC_TRUE;
    ALC_CALL(alcMakeContextCurrent, contextMadeCurrent, m_internal->device, nullptr);
    ALC_CALL(alcDestroyContext, m_internal->device, m_internal->context);
    ALCboolean closed;
    ALC_CALL(alcCloseDevice, closed, m_internal->device, m_internal->device);
}
}  // namespace de
