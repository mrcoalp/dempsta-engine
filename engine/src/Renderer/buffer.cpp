#include "Renderer/buffer.h"

#include "Renderer/renderer.h"
#include "Platform/OpenGL/openglbuffer.h"
#include "Core/log.h"

namespace de {
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
        switch (Renderer::GetApi()) {
            case RendererAPI::NONE: {
                LOG_ENGINE_ERROR("NONE, for now, is not a valid renderer API!");
                return nullptr;
            }
            case RendererAPI::OPENGL: {
                return new OpenGLVertexBuffer(vertices, size);
            }
            default: {
                LOG_ENGINE_ERROR("Not a valid renderer API was provided!");
                return nullptr;
            }
        }
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
        switch (Renderer::GetApi()) {
            case RendererAPI::NONE: {
                LOG_ENGINE_ERROR("NONE, for now, is not a valid renderer API!");
                return nullptr;
            }
            case RendererAPI::OPENGL: {
                return new OpenGLIndexBuffer(indices, count);
            }
            default: {
                LOG_ENGINE_ERROR("Not a valid renderer API was provided!");
                return nullptr;
            }
        }
    }
}
