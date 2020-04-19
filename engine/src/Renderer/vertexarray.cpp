#include "Renderer/vertexarray.h"

#include "Core/log.h"
#include "Platform/OpenGL/openglvertexarray.h"
#include "Renderer/renderer.h"

namespace de {
VertexArray* VertexArray::Create() {
    switch (Renderer::GetApi()) {
        case RendererAPI::API::None: {
            LOG_ENGINE_ERROR("NONE, for now, is not a valid renderer API!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: {
            return new OpenGLVertexArray();
        }
        default: {
            LOG_ENGINE_ERROR("Not a valid renderer API was provided!");
            return nullptr;
        }
    }
}
}  // namespace de
