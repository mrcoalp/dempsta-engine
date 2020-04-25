#include "Renderer/shader.h"

#include "Core/log.h"
#include "Platform/OpenGL/openglshader.h"
#include "Renderer/renderer.h"

namespace de {
Ref<Shader> Shader::Create(const std::string& filePath) {
    switch (Renderer::GetApi()) {
        case RendererAPI::API::None: {
            LOG_ENGINE_ERROR("NONE, for now, is not a valid renderer API!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: {
            return std::make_shared<OpenGLShader>(filePath);
        }
        default: {
            LOG_ENGINE_ERROR("Not a valid renderer API was provided!");
            return nullptr;
        }
    }
}

Ref<Shader> Shader::Create(const std::string& vertexSource, const std::string& fragmentSource) {
    switch (Renderer::GetApi()) {
        case RendererAPI::API::None: {
            LOG_ENGINE_ERROR("NONE, for now, is not a valid renderer API!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: {
            return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
        }
        default: {
            LOG_ENGINE_ERROR("Not a valid renderer API was provided!");
            return nullptr;
        }
    }
}
}  // namespace de
