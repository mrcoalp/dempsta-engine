#include "Platform/OpenAL/openalsoundapi.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "Platform/OpenAL/openalutils.h"

namespace de {
/// Max number of sources that can play simultaneously.
constexpr size_t MAX_NUMBER_OF_SOURCES{32};

OpenALSoundAPI::OpenALSoundAPI() : m_internal(CreateScope<Internal>()) {}

void OpenALSoundAPI::Init() {
    m_internal->device = alcOpenDevice(nullptr);
    DE_ASSERT(m_internal->device != nullptr, "Could not open ALC device!")
    DE_ASSERT(ALC_CALL(alcCreateContext, m_internal->context, m_internal->device, m_internal->device, nullptr) && m_internal->context != nullptr,
              "Could not create audio context!")
    ALCboolean contextMadeCurrent = ALC_FALSE;
    DE_ASSERT(ALC_CALL(alcMakeContextCurrent, contextMadeCurrent, m_internal->device, m_internal->context) && contextMadeCurrent == ALC_TRUE,
              "Could not make audio context current!")

    m_allSources.resize(MAX_NUMBER_OF_SOURCES);
    AL_CALL(alGenSources, MAX_NUMBER_OF_SOURCES, m_allSources.data());
    m_availableSources = m_allSources;
}

bool OpenALSoundAPI::CheckAndGetAvailableSource(uint32_t& source) {
    if (m_availableSources.empty()) {
        LOG_ENGINE_ERROR("Maximum number of simultaneous sounds playing reached! Aborted...");
        return false;
    }

    source = m_availableSources.back();
    m_availableSources.pop_back();
    m_sourcesInUse.push_back(source);

    return true;
}

void OpenALSoundAPI::ReleaseSource(uint32_t source) {
    m_sourcesInUse.erase(std::remove(m_sourcesInUse.begin(), m_sourcesInUse.end(), source), m_sourcesInUse.end());
    m_availableSources.push_back(source);
}

void OpenALSoundAPI::Destroy() {
    for (const auto& source : m_allSources) {
        AL_CALL(alSourceStop, source);
        AL_CALL(alSourcei, source, AL_BUFFER, AL_NONE);
    }
    AL_CALL(alDeleteSources, MAX_NUMBER_OF_SOURCES, m_allSources.data());
    // TODO(mpinto): Replace this with a proper sound manager to unload all instances
    for (const auto& destroyInstance : m_destroyInstanceCallbacks) {
        destroyInstance();
    }
    ALCboolean contextMadeCurrent = ALC_TRUE;
    ALC_CALL(alcMakeContextCurrent, contextMadeCurrent, m_internal->device, nullptr);
    ALC_CALL(alcDestroyContext, m_internal->device, m_internal->context);
    ALCboolean closed;
    ALC_CALL(alcCloseDevice, closed, m_internal->device, m_internal->device);
}
}  // namespace de
