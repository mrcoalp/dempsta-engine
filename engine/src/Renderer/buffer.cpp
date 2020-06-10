#include "Renderer/buffer.h"

#include "Core/log.h"
#include "Platform/OpenGL/openglbuffer.h"
#include "Renderer/renderer.h"

namespace de {
Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    switch (Renderer::GetApi()) {
        case RendererAPI::API::None: {
            LOG_ENGINE_ERROR("NONE, for now, is not a valid renderer API!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: {
            return std::make_shared<OpenGLVertexBuffer>(size);
        }
        default: {
            LOG_ENGINE_ERROR("Not a valid renderer API was provided!");
            return nullptr;
        }
    }
}

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
    switch (Renderer::GetApi()) {
        case RendererAPI::API::None: {
            LOG_ENGINE_ERROR("NONE, for now, is not a valid renderer API!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: {
            return std::make_shared<OpenGLVertexBuffer>(vertices, size);
        }
        default: {
            LOG_ENGINE_ERROR("Not a valid renderer API was provided!");
            return nullptr;
        }
    }
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
    switch (Renderer::GetApi()) {
        case RendererAPI::API::None: {
            LOG_ENGINE_ERROR("NONE, for now, is not a valid renderer API!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: {
            return std::make_shared<OpenGLIndexBuffer>(indices, count);
        }
        default: {
            LOG_ENGINE_ERROR("Not a valid renderer API was provided!");
            return nullptr;
        }
    }
}
}  // namespace de
