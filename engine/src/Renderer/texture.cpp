#include "Renderer/texture.h"

#include "Core/log.h"
#include "Platform/OpenGL/opengltexture.h"
#include "Renderer/renderer.h"

namespace de {
Ref<Texture2D> Texture2D::Create(const std::string& filePath) {
    switch (Renderer::GetApi()) {
        case RendererAPI::API::None: {
            LOG_ENGINE_ERROR("NONE, for now, is not a valid renderer API!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: {
            return std::make_shared<OpenGLTexture2D>(filePath);
        }
        default: {
            LOG_ENGINE_ERROR("Not a valid renderer API was provided!");
            return nullptr;
        }
    }
}
}  // namespace de
