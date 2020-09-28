#include "Platform/OpenAL/openalutils.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "Core/log.h"

namespace de {
bool OpenALUtils::check_al_errors(const std::string& filename, const std::uint_fast32_t line) {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        LOG_ENGINE_ERROR("AL ERROR: ({}: {})", filename, line);
        switch (error) {
            case AL_INVALID_NAME:
                LOG_ENGINE_ERROR("AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function");
                break;
            case AL_INVALID_ENUM:
                LOG_ENGINE_ERROR("AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function");
                break;
            case AL_INVALID_VALUE:
                LOG_ENGINE_ERROR("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
                break;
            case AL_INVALID_OPERATION:
                LOG_ENGINE_ERROR("AL_INVALID_OPERATION: the requested operation is not valid");
                break;
            case AL_OUT_OF_MEMORY:
                LOG_ENGINE_ERROR("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory");
                break;
            default:
                LOG_ENGINE_ERROR("UNKNOWN AL ERROR: {}", error);
        }
        return false;
    }
    return true;
}

bool OpenALUtils::check_alc_errors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device) {
    ALCenum error = alcGetError(device);
    if (error != ALC_NO_ERROR) {
        LOG_ENGINE_ERROR("ALC ERROR: ({}: {})", filename, line);
        switch (error) {
            case ALC_INVALID_VALUE:
                LOG_ENGINE_ERROR("ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function");
                break;
            case ALC_INVALID_DEVICE:
                LOG_ENGINE_ERROR("ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function");
                break;
            case ALC_INVALID_CONTEXT:
                LOG_ENGINE_ERROR("ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function");
                break;
            case ALC_INVALID_ENUM:
                LOG_ENGINE_ERROR("ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function");
                break;
            case ALC_OUT_OF_MEMORY:
                LOG_ENGINE_ERROR("ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function");
                break;
            default:
                LOG_ENGINE_ERROR("UNKNOWN ALC ERROR: {}", error);
        }
        return false;
    }
    return true;
}
}  // namespace de
