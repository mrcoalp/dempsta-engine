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
            return CreateRef<OpenGLShader>(filePath);
        }
        default: {
            LOG_ENGINE_ERROR("Not a valid renderer API was provided!");
            return nullptr;
        }
    }
}

Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSource,
                           const std::string& fragmentSource) {
    switch (Renderer::GetApi()) {
        case RendererAPI::API::None: {
            LOG_ENGINE_ERROR("NONE, for now, is not a valid renderer API!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: {
            return CreateRef<OpenGLShader>(name, vertexSource, fragmentSource);
        }
        default: {
            LOG_ENGINE_ERROR("Not a valid renderer API was provided!");
            return nullptr;
        }
    }
}

void ShaderLibrary::Add(const Ref<Shader>& shader) { Add(shader->GetName(), shader); }

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
    DE_ASSERT(!Exists(name), "Shader '{0}' already exists!", name)
    m_shaders[name] = shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
    auto _shader = Shader::Create(filepath);
    Add(_shader);
    return _shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
    auto _shader = Shader::Create(filepath);
    Add(name, _shader);
    return _shader;
}
const Ref<Shader>& ShaderLibrary::Get(const std::string& name) const {
    DE_ASSERT(Exists(name), "Shader '{0}' not found!", name)
    return m_shaders.at(name);
}
}  // namespace de
