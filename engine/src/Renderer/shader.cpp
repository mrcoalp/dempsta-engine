#include "Renderer/shader.h"

#include "Core/log.h"
#include "Platform/OpenGL/openglshader.h"
#include "Renderer/renderer.h"

namespace de {
Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource) {
    switch (Renderer::GetApi()) {
        case RendererAPI::API::None: {
            LOG_ENGINE_ERROR("NONE, for now, is not a valid renderer API!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: {
            return new OpenGLShader(vertexSource, fragmentSource);
        }
        default: {
            LOG_ENGINE_ERROR("Not a valid renderer API was provided!");
            return nullptr;
        }
    }
}
}  // namespace de
