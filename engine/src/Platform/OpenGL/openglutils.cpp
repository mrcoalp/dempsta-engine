#include "Platform/OpenGL/openglutils.h"

#include <glad/glad.h>

#include "Core/log.h"

namespace de {
bool OpenGLUtils::check_gl_errors(const std::string& filename, std::uint_fast32_t line) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        LOG_ENGINE_ERROR("** GL ERROR ** - {}: {}", filename, line);
        return false;
    }
    return true;
}
}  // namespace de
